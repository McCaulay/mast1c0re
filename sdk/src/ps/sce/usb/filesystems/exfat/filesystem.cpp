#if (defined(PS4) && PS4)
#include <ps/sce/usb/filesystems/exfat/filesystem.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/filesystems/exfat/entry.hpp>
#include <ps/ps.hpp>

exFAT::Filesystem::Filesystem()
{
    this->mounted = false;
    this->massStore = nullptr;
    this->baseAddress = 0;
    this->clusterSize = 0;
    this->fatSize = 0;
    this->fatBase = 0;
    this->dataBase = 0;

    this->cacheIndex = 0xFFFFFFFF;

    this->readStartClusterIndex = 0;
    this->readClusterIndex = 0;
}

exFAT::Filesystem::Filesystem(PS::MassStore* massStore, uint32_t baseAddress, exFAT::BootSector bootSector)
{
    this->mounted = false;
    this->massStore = massStore;
    this->baseAddress = baseAddress;
    this->bootSector = bootSector;
    this->clusterSize = 0;
    this->fatSize = 0;
    this->fatBase = 0;
    this->dataBase = 0;

    this->cacheIndex = 0xFFFFFFFF;

    this->readStartClusterIndex = 0;
    this->readClusterIndex = 0;
}

bool exFAT::Filesystem::isMounted()
{
    return this->mounted;
}

bool exFAT::Filesystem::mount()
{
    if (this->isMounted())
        return true;

    if (this->massStore == nullptr)
        return false;

    // Bytes per sector must be 512
    if (this->bootSector.getBytesPerSector() != SECTOR_SIZE)
        return false;

    // 1 FAT must be present for removable media
    if (this->bootSector.numberOfFATs != 1)
        return false;

    this->clusterSize = this->bootSector.getClusterSize();
    this->fatSize = this->bootSector.getFatSize();
    this->fatBase = this->bootSector.getFatBase();
    this->dataBase = this->bootSector.getDataBase();

    this->mounted = true;
    return true;
}

uint32_t exFAT::Filesystem::getClusterSize()
{
    return this->clusterSize;
}

bool exFAT::Filesystem::root(List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    return this->directory(this->bootSector.rootCluster, directories, files);
}

bool exFAT::Filesystem::directory(const char* filepath, List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    if (filepath == "/")
        return this->root(directories, files);

    exFAT::Directory parent;
    if (!this->getDirectoryFromFilepath(filepath, &parent))
        return false;

    if (this->directory(parent, directories, files))
        return true;
    return false;
}

bool exFAT::Filesystem::directory(exFAT::Directory parent, List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    return this->directory(parent.getCluster(), directories, files);
}

List<exFAT::Directory> exFAT::Filesystem::directories(const char* filepath)
{
    List<exFAT::Directory> directories;
    this->directory(filepath, &directories);
    return directories;
}

List<exFAT::Directory> exFAT::Filesystem::directories(exFAT::Directory parent)
{
    List<exFAT::Directory> directories;
    this->directory(parent, &directories);
    return directories;
}

List<exFAT::File> exFAT::Filesystem::files(const char* filepath)
{
    List<exFAT::File> files;
    this->directory(filepath, nullptr, &files);
    return files;
}

List<exFAT::File> exFAT::Filesystem::files(exFAT::Directory parent)
{
    List<exFAT::File> files;
    this->directory(parent, nullptr, &files);
    return files;
}

bool exFAT::Filesystem::directory(uint32_t cluster, List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    if (!this->isMounted() || (directories == nullptr && files == nullptr))
        return false;

    uint32_t bytesPerCluster = (uint64_t)this->bootSector.getClusterSize();

    // Loop each sector in cluster
    for (uint32_t i = 0; i < this->bootSector.getSectorsPerCluster(); i++)
    {
        // Read current sector in current cluster
        uint8_t sector[SECTOR_SIZE];
        if (!this->readSector(this->getSectorIndexForCluster(cluster) + i, sector))
            continue;

        // Loop each directory entry
        for (uint32_t j = 0; j < SECTOR_SIZE; j += sizeof(exFAT::Entry))
        {
            // Get base entry information
            exFAT::Entry baseEntry = *(exFAT::Entry*)(sector + j);

            // Ignore empty / unused entries / all types that are not file/directory
            if (baseEntry.isEndOfDirectory() || baseEntry.isUnused() || baseEntry.getType() != EXFAT_ENTRY_TYPE_CODE_FILE_AND_DIRECTORY)
                continue;

            // Get entry as file/directory
            exFAT::FileDirectoryEntry entry = *(exFAT::FileDirectoryEntry*)(sector + j);

            // Ignore system files
            if ((entry.fileAttributes & EXFAT_FILE_ATTRIBUTE_SYSTEM) == EXFAT_FILE_ATTRIBUTE_SYSTEM)
                continue;

            // Move to next entry
            j += sizeof(exFAT::Entry);
            exFAT::Entry baseEntryStream = *(exFAT::Entry*)(sector + j);

            // Ignore empty / unused entries / or not stream extension
            if (baseEntryStream.isEndOfDirectory() || baseEntryStream.isUnused() || baseEntryStream.getType() != EXFAT_ENTRY_TYPE_CODE_STREAM_EXTENSION)
                continue;

            // Get entry as stream extension
            exFAT::StreamExtensionEntry stream = *(exFAT::StreamExtensionEntry*)(sector + j);

            // Move to next entry
            j += sizeof(exFAT::Entry);
            exFAT::Entry baseEntryFilename = *(exFAT::Entry*)(sector + j);

            // Get filename entries
            char filename[256];
            PS2::memset(filename, 0, sizeof(filename));
            while (!baseEntryFilename.isEndOfDirectory() && !baseEntryFilename.isUnused() && baseEntryFilename.getType() == EXFAT_ENTRY_TYPE_CODE_FILE_NAME)
            {
                exFAT::FilenameEntry filenameEntry = *(exFAT::FilenameEntry*)(sector + j);

                // Append filename
                this->strcatwn(filename, filenameEntry.filename, sizeof(filenameEntry.filename));

                j += sizeof(exFAT::Entry);
                baseEntryFilename = *(exFAT::Entry*)(sector + j);
            }
            // Rewind one entry
            j -= sizeof(exFAT::Entry);

            // Directory
            if ((entry.fileAttributes & EXFAT_FILE_ATTRIBUTE_DIRECTORY) == EXFAT_FILE_ATTRIBUTE_DIRECTORY)
            {
                if (directories != nullptr)
                    directories->add(exFAT::Directory(filename, entry, stream, bytesPerCluster));
                continue;
            }

            // File
            if (files != nullptr)
                files->add(exFAT::File(filename, entry, stream, bytesPerCluster));
        }
    }

    // Sort directories by name
    if (directories != nullptr)
        this->sortDirectories(directories);

    // Sort files by name
    if (files != nullptr)
        this->sortFiles(files);

    return true;
}

bool exFAT::Filesystem::getDirectoryFromFilepath(const char* filepath, exFAT::Directory* directory)
{
    if (directory == nullptr)
        return false;

    char bFilepath[256];
    PS2::memset(bFilepath, 0, sizeof(bFilepath));
    PS2::strcpy(bFilepath, filepath);
    uint32_t length = PS2::strlen(filepath);

    // Append trailing "/"
    if (bFilepath[length - 1] != '/')
    {
        bFilepath[length] = '/';
        length++;
    }

    uint32_t offsetStart = 0;
    uint32_t offsetEnd = 0;
    bool isRoot = true;
    bool foundTarget = false;

    for (uint32_t i = 0; i < length; i++)
    {
        if (bFilepath[i] != '/')
            continue;

        offsetEnd = i;
        if (i != 0 && offsetEnd > offsetStart)
        {
            // Get sub directories
            List<exFAT::Directory> directories;
            if (isRoot)
            {
                if (!this->root(&directories, nullptr))
                    return false;
                isRoot = false;
            }
            else
            {
                if (!this->directory(*directory, &directories, nullptr))
                    return false;
            }

            // Find directory name in directories
            bool found = false;
            for (uint32_t j = 0; j < directories.size(); j++)
            {
                if (PS2::memcmp(directories[j].getName(), (void*)(bFilepath + offsetStart), offsetEnd - offsetStart) != 0)
                    continue;
                *directory = directories[j];
                found = true;
                foundTarget = true;
            }
            if (!found)
                return false;
        }
        offsetStart = offsetEnd + 1;
    }
    return foundTarget;
}

bool exFAT::Filesystem::exists(const char* filepath)
{
    if (filepath == "/")
        return true;

    exFAT::Directory parent;
    return this->getDirectoryFromFilepath(filepath, &parent);
}

bool exFAT::Filesystem::resetRead(exFAT::File file)
{
    this->readStartClusterIndex = file.getCluster();
    this->readClusterIndex = this->readStartClusterIndex;
    return this->readClusterIndex != EXFAT_END_OF_CLUSTER_CHAIN;
}

bool exFAT::Filesystem::readNextCluster(exFAT::File file, uint8_t* buffer)
{
    if (this->readClusterIndex == EXFAT_END_OF_CLUSTER_CHAIN)
        return false;

    if (file.isContinuous())
    {
        uint32_t clusterCount = this->readClusterIndex - this->readStartClusterIndex;
        if (clusterCount >= file.getClusterCount())
            return false;
    }

    uint32_t sectorIndex = this->getSectorIndexForCluster(this->readClusterIndex);
    bool valid = this->readCluster(sectorIndex, buffer);
    if (!valid)
        PS::Debug.printf("Failed to read cluster at sector index: %i\n", sectorIndex);

    if (file.isContinuous())
        this->readClusterIndex++;
    else
        this->readClusterIndex = this->getNextCluster(this->readClusterIndex);
    return valid;
}

bool exFAT::Filesystem::readCluster(uint32_t blockOffset, uint8_t* buffer)
{
    // Read sectors in chunks
    uint32_t sectorsPerCluster = this->bootSector.getSectorsPerCluster();
    uint32_t bytesPerSector = this->bootSector.getBytesPerSector();
    uint32_t offset = 0;
    uint32_t chunkSize = 96;
    for (uint32_t i = 0; i < sectorsPerCluster; i += chunkSize)
    {
        // Get remainder
        uint32_t sectorsRemaining = chunkSize;
        if (i + sectorsRemaining > sectorsPerCluster)
            sectorsRemaining = sectorsPerCluster % sectorsRemaining;
        if (!this->readSectors(blockOffset + i, buffer + offset, sectorsRemaining))
            return false;
        offset += (bytesPerSector * sectorsRemaining);
    }
    return true;
}

bool exFAT::Filesystem::cacheSector(uint32_t blockOffset)
{
    // Cache exists
    if (blockOffset == this->cacheIndex)
        return true;

    if (this->readSector(blockOffset, this->cache))
    {
        this->cacheIndex = blockOffset;
        return true;
    }
    return false;
}

uint32_t exFAT::Filesystem::getNextCluster(uint32_t cluster)
{
    uint32_t offset = cluster * sizeof(uint32_t);
    uint32_t sectorOffset = 0;
    while (offset >= SECTOR_SIZE)
    {
        offset -= SECTOR_SIZE;
        sectorOffset++;
    }

    this->cacheSector(this->fatBase + sectorOffset);
    return *(uint32_t*)(this->cache + offset);
}

void exFAT::Filesystem::sortDirectories(List<exFAT::Directory>* directories)
{
    if (directories == nullptr)
        return;

    for (uint32_t i = 0; i < directories->size() - 1; i++)
    {
        bool hasSwapped = false;

        for (uint32_t j = 0; j < directories->size() - i - 1; j++)
        {
            if (PS2::strcmp((*directories)[j].getName(), (*directories)[j + 1].getName()) > 0)
            {
                exFAT::Directory temp = (*directories)[j];
                (*directories)[j] = (*directories)[j + 1];
                (*directories)[j + 1] = temp;

                hasSwapped = true;
            }
        }

        if (!hasSwapped)
            break;
    }
}

void exFAT::Filesystem::sortFiles(List<exFAT::File>* files)
{
    if (files == nullptr)
        return;

    for (uint32_t i = 0; i < files->size() - 1; i++)
    {
        bool hasSwapped = false;

        for (uint32_t j = 0; j < files->size() - i - 1; j++)
        {
            if (PS2::strcmp((*files)[j].getName(), (*files)[j + 1].getName()) > 0)
            {
                exFAT::File temp = (*files)[j];
                (*files)[j] = (*files)[j + 1];
                (*files)[j + 1] = temp;

                hasSwapped = true;
            }
        }

        if (!hasSwapped)
            break;
    }
}

char* exFAT::Filesystem::strcatwn(char* destination, uint8_t* source, uint32_t slen)
{
    uint32_t offset = PS2::strlen(destination);
    for (uint32_t i = 0; i < slen; i += 2)
    {
        destination[offset] = source[i] <= 0x7f ? source[i] : '?';
        offset++;
    }
    return destination;
}
#endif
#endif