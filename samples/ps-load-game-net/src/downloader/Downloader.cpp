#include "Downloader.hpp"

bool Downloader::download(const char* filepath, uint16_t port)
{
    // Create TCP server
    PS::TcpServer server = PS::TcpServer();
    if (!server.listen(port))
        return false;

    // Show progress bar dialog
    PS::Sce::MsgDialogProgressBar dialog = PS::Sce::MsgDialogProgressBar("Waiting for connection...");
    dialog.open();

    // Accept connection
    PS::Debug.printf("Waiting for client connection...\n");
    PS::TcpClient client = server.accept();

    // Get magic
    PS::Debug.printf("Waiting for magic...\n");
    if (client.read<uint32_t>() != MAGIC)
        return false;

    // Get filesize
    PS::Debug.printf("Waiting for data size...\n");
    size_t filesize = client.read<size_t>();
    PS::Debug.printf("Download file of size: %llu\n", filesize);

    // Get file basename
    const char* basename = PS::Filesystem::basename(filepath);
    Downloader::setProgress(dialog, basename, 0, filesize);

    // Write file to device
    PS::Debug.printf("Opening %s\n", filepath);
    int fd = PS::open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd > 0)
    {
        // Write file to disk from socket in chunks
        uint32_t updateBar = 0;
        char buffer[DOWNLOAD_CHUNK_SIZE];
        for (uint64_t i = 0; i < filesize; i+= DOWNLOAD_CHUNK_SIZE)
        {
            uint32_t readWriteSize = DOWNLOAD_CHUNK_SIZE;

            // Set size for final read if we are at the end
            if (i + readWriteSize > filesize)
                readWriteSize = filesize % DOWNLOAD_CHUNK_SIZE;

            size_t readCount = client.read(buffer, readWriteSize);
            if (readCount != readWriteSize)
            {
                PS::Debug.printf("Failed to read from socket, read %llu, expected to read %llu\n", readCount, readWriteSize);
                PS::close(fd);
                client.disconnect();
                server.disconnect();
                dialog.close();
                return false;
            }

            size_t writeCount = PS::writeAll(fd, buffer, readWriteSize);
            if (writeCount != readWriteSize)
            {
                PS::Debug.printf("Failed to write file, wrote %llu, expected to write %llu\n", writeCount, readWriteSize);
                PS::close(fd);
                client.disconnect();
                server.disconnect();
                dialog.close();
                return false;
            }

            // Update progress bar
            if (updateBar == DOWNLOAD_BAR_UPDATE_FREQUENCY)
            {
                Downloader::setProgress(dialog, basename, i, filesize);
                updateBar = 0;
            }
            updateBar++;
        }
    }

    // Disconnect
    PS::Debug.printf("Closing connection...\n");
    PS::close(fd);
    client.disconnect();
    server.disconnect();
    dialog.terminate();

    // Validate filesize matches expected
    size_t filesystemFilesize = PS::Filesystem::getFileSize(filepath);
    if (filesystemFilesize != filesize)
    {
        PS::Debug.printf("Wrote a file which resulted in a different size to what was expected. %llu was wrote, however we expected to write %llu\n", filesystemFilesize, filesize);
        return false;
    }
    
    return true;
}

void Downloader::setProgress(PS::Sce::MsgDialogProgressBar dialog, const char* basename, size_t downloaded, size_t total)
{
    dialog.setMsg("Downloading %s...\n", basename);

    // Calculate percentage without float/double
    uint64_t divident = downloaded * 100;
    uint64_t percentage = 0;

    while (divident >= total)
    {
        divident -= total;
        percentage++;
    }

    dialog.setValue(percentage);
}