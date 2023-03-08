#pragma once

#include <types.hpp>

#define MBR_PARTITION_TYPE_EMPTY 0x00

#pragma pack(push, 1)
class MasterBootRecord
{
public:
    inline bool isEmpty()
    {
        return this->partitionType == MBR_PARTITION_TYPE_EMPTY || this->logicalBlockAddress == 0;
    }
public:
    uint8_t bootIndicator;
    uint8_t startHead;
    uint8_t startSector;
    uint8_t startTrack;
    uint8_t partitionType;
    uint8_t endHead;
    uint8_t endSector;
    uint8_t endTrack;
    uint32_t logicalBlockAddress;
    uint32_t sectorCount;
};
#pragma pack(pop)