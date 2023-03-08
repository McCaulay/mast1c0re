#pragma once

#include <types.hpp>
#include <ps/sce/usb/filesystems/mbr/master-boot-record.hpp>

#define MBR_SIGNATURE 0xaa55
#define MBR_MAX_PARTITIONS 4

#pragma pack(push, 1)
class MasterBootRecordTable
{
public:
    inline bool isValid()
    {
        return this->signature == MBR_SIGNATURE;
    }

    inline uint32_t getMaxParitionCount()
    {
        return MBR_MAX_PARTITIONS;
    }

    inline uint16_t getSignature()
    {
        return this->signature;
    }

    inline MasterBootRecord getParition(uint32_t index)
    {
        return this->partitions[index];
    }
public:
    uint8_t bootstrapCode[446];
    MasterBootRecord partitions[MBR_MAX_PARTITIONS];
    uint16_t signature;
};
#pragma pack(pop)