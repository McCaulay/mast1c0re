#if (defined(PS4) && PS4)
#include <ps/sce/usb/filesystems/exfat/file.hpp>
#ifdef LIBKERNEL
#include <ps2/ps2.hpp>

exFAT::File::File() : exFAT::Record::Record()
{
    
}

exFAT::File::File(char* name, exFAT::FileDirectoryEntry entry, exFAT::StreamExtensionEntry stream, uint32_t bytesPerCluster) : exFAT::Record::Record(name, entry, stream, bytesPerCluster)
{
    
}

bool exFAT::File::hasExtension(const char* extension)
{
    char* filename = this->getName();
    int index = PS2::lastIndexOf(filename, '.');
    if (index == -1)
        return false;
    return PS2::strcmp((filename + index + 1), extension) == 0;
}
#endif
#endif