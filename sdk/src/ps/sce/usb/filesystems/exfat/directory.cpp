#if (defined(PS4) && PS4)
#include <ps/sce/usb/filesystems/exfat/directory.hpp>
#ifdef LIBKERNEL
exFAT::Directory::Directory() : exFAT::Record::Record()
{
    
}

exFAT::Directory::Directory(char* name, exFAT::FileDirectoryEntry entry, exFAT::StreamExtensionEntry stream, uint32_t bytesPerCluster) : exFAT::Record::Record(name, entry, stream, bytesPerCluster)
{
    
}
#endif
#endif