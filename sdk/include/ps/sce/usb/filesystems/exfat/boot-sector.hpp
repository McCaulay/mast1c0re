#pragma once

#include <types.hpp>

namespace exFAT
{
    #pragma pack(push, 1)
    class BootSector
    {
    public:
        inline uint32_t getBytesPerSector()
        {
            return 1 << this->bytesPerSectorShift;
        }

        inline uint32_t getSectorsPerCluster()
        {
            return 1 << this->sectorsPerClusterShift;
        }

        inline uint32_t getClusterSize()
        {
            return this->getBytesPerSector() * this->getSectorsPerCluster();
        }

        inline uint32_t getFatSize()
        {
            return this->getBytesPerSector() * this->sectorsPerFAT;
        }

        inline uint32_t getFatBase()
        {
            return this->fatSectorOffset;
        }

        inline uint32_t getDataBase()
        {
            return this->clusterHeapOffset;
        }
    public:
        uint8_t  jumpCode[3];            // 0x00
        char     filesystemName[8];      // 0x03
        uint8_t  zero[53];               // 0x0b: Zeros filling BPB of FAT filesystem
        uint64_t partitionSectorOffset;  // 0x40
        uint64_t volumeLength;           // 0x48
        uint32_t fatSectorOffset;        // 0x50
        uint32_t sectorsPerFAT;          // 0x54
        uint32_t clusterHeapOffset;      // 0x58
        uint32_t clusterCount;           // 0x5c
        uint32_t rootCluster;            // 0x60
        uint32_t serialNumber;           // 0x64
        uint16_t revision;               // 0x68
        uint16_t volumeFlags;            // 0x6a
        uint8_t  bytesPerSectorShift;    // 0x6c
        uint8_t  sectorsPerClusterShift; // 0x6d
        uint8_t  numberOfFATs;           // 0x6e
        uint8_t  driveSelect;            // 0x6f
        uint8_t  percentInUse;           // 0x70
        uint8_t  reserved[7];            // 0x71
        uint8_t  bootstrapCode[390];     // 0x78
        uint16_t signature;              // 0x1fe
    };
    #pragma pack(pop)
}