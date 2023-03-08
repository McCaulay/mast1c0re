#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/filesystems/exfat/entry.hpp>

namespace exFAT
{
    class Record
    {
    public:
        Record();
        Record(char* name, exFAT::FileDirectoryEntry entry, exFAT::StreamExtensionEntry stream, uint32_t bytesPerCluster);

        inline char* getName()
        {
            return this->name;
        }

        inline uint32_t getCluster()
        {
            return this->stream.firstCluster;
        }

        inline uint32_t getClusterCount()
        {
            return this->clusterCount;
        }

        inline uint64_t getSize()
        {
            return this->stream.length;
        }

        inline bool isContinuous()
        {
            return (this->stream.generalSecondaryFlags & EXFAT_SECONDARY_NO_FAT_CHAIN) == EXFAT_SECONDARY_NO_FAT_CHAIN;
        }
    protected:
        char name[256];
        uint32_t clusterCount;
        exFAT::FileDirectoryEntry entry;
        exFAT::StreamExtensionEntry stream;
    };
}
#endif
#endif