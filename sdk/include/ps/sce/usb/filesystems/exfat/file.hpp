#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/filesystems/exfat/record.hpp>
#include <ps/sce/usb/filesystems/exfat/entry.hpp>

namespace exFAT
{
    class File : public Record
    {
    public:
        File();
        File(char* name, exFAT::FileDirectoryEntry entry, exFAT::StreamExtensionEntry stream, uint32_t bytesPerCluster);
        bool hasExtension(const char* extension);
    };
}
#endif
#endif