#if (defined(PS4) && PS4)
#include <ps/sce/usb/filesystems/exfat/record.hpp>
#ifdef LIBKERNEL
#include <ps2/ps2.hpp>

exFAT::Record::Record()
{
    PS2::memset(this->name, 0, sizeof(this->name));
    this->clusterCount = 0;
}

exFAT::Record::Record(char* name, exFAT::FileDirectoryEntry entry, exFAT::StreamExtensionEntry stream, uint32_t bytesPerCluster)
{
    PS2::memset(this->name, 0, sizeof(this->name));
    PS2::strcpy(this->name, name);
    this->entry = entry;
    this->stream = stream;

    this->clusterCount = 0;
    for (uint64_t i = 0; i < this->stream.length; i += bytesPerCluster)
        this->clusterCount++;
    if (this->stream.length > 0)
        this->clusterCount++;
}
#endif
#endif