#include "Dumper.hpp"

#define CHUNK_SIZE 4096

bool Dumper::dumpDirectory(PS::TcpClient* client, const char* dirpath)
{
    PS::Debug.printf("Dumping %s...\n", dirpath);

    // Open directory
    int fd = PS::open(dirpath, 0, 0);
    if (fd <= 0)
    {
        PS::Sce::MsgDialogUserMessage::show("Failed to open common/lib directory!");
        return false;
    }

    // Iterate directories/files
    char buffer[8192];
    int nread = 0;
    while ((nread = PS::getdents(fd, buffer, sizeof(buffer))) > 0)
    {
        struct dirent* dent = (struct dirent*)buffer;
        while (dent->d_fileno)
        {
            if (dent->d_type == DT_REG)
                Dumper::dumpFile(client, dirpath, dent->d_name);

            if (dent->d_reclen == 0)
                break;
            nread -= dent->d_reclen;
            if (nread < 0)
                break;
            dent = (struct dirent*)((char*)dent + dent->d_reclen);
        }
        PS2::memset(buffer, 0, sizeof(buffer));
    }

    PS::close(fd);
    return true;
}

bool Dumper::dumpFile(PS::TcpClient* client, const char* dirpath, const char* filename)
{
    char filepath[MAX_PATH];
    PS2::memset(filepath, 0, MAX_PATH);
    PS2::strcpy(filepath, dirpath);
    PS2::strcat(filepath, filename);

    int filenameLen = PS2::strlen(filename);

    // Send filename (tag/length/value)
    uint32_t PROTO_TAG_FILENAME = 0x41c11d9c;
    client->send(&PROTO_TAG_FILENAME, sizeof(PROTO_TAG_FILENAME));
    client->send(&filenameLen, sizeof(int));
    client->send((void*)filename, filenameLen);

    // Get filesize
    size_t filesize = PS::Filesystem::getFileSize(filepath);
    PS::Debug.printf("- %s (%llub)\n", filename, filesize);

    // Read file
    int32_t fd = PS::open(filepath, O_RDONLY, 0);
    if (fd <= 0)
    {
        PS::Debug.printf("Error: Failed to open %s\n", filepath);
        return false;
    }

    // Send file contents
    uint32_t PROTO_TAG_FILEDATA = 0x55e58570;
    client->send(&PROTO_TAG_FILEDATA, sizeof(PROTO_TAG_FILEDATA));
    client->send(&filesize, sizeof(filesize));

    // Send in chunks
    bool errorOccured = false;
    uint8_t buffer[CHUNK_SIZE];
    for (uint64_t i = 0; i < filesize; i += CHUNK_SIZE)
    {
        // Get remainder
        uint32_t copySize = CHUNK_SIZE;
        if (i + copySize > filesize)
            copySize = filesize % copySize;

        // Read from file
        size_t readCount = PS::readAll(fd, buffer, copySize);
        if (readCount != copySize)
        {
            PS::Debug.printf("Failed to read file %s, read %llu, expected to read %llu\n", filepath, readCount, copySize);
            errorOccured = true;
            break;
        }

        // Send to server
        size_t sentCount = client->send(buffer, copySize);
        if (sentCount != copySize)
        {
            PS::Debug.printf("Failed to send file %s, sent %llu, expected to send %llu\n", filepath, sentCount, copySize);
            errorOccured = true;
            break;
        }
    }

    PS::close(fd);
    return !errorOccured;
}