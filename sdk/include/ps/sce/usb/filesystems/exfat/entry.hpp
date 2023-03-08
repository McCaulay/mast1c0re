#pragma once

#include <types.hpp>

#define EXFAT_ENTRY_TYPE_CODE_ALLOCATION_BITMAP               0x81
#define EXFAT_ENTRY_TYPE_CODE_UP_CASE_TABLE                   0x82
#define EXFAT_ENTRY_TYPE_CODE_VOLUME_LABEL                    0x83
#define EXFAT_ENTRY_TYPE_CODE_FILE_AND_DIRECTORY              0x85
#define EXFAT_ENTRY_TYPE_CODE_STREAM_EXTENSION                0xc0
#define EXFAT_ENTRY_TYPE_CODE_FILE_NAME                       0xc1
#define EXFAT_ENTRY_TYPE_CODE_WINDOWS_CE_ACCESS_CONTROL_LIST  0xc2
#define EXFAT_ENTRY_TYPE_CODE_VOLUME_GUID                     0xa0
#define EXFAT_ENTRY_TYPE_CODE_TEXFAT_PADDING                  0xa1
#define EXFAT_ENTRY_TYPE_CODE_WINDOWS_CE_ACCESS_CONTROL_TABLE 0xa2

#define EXFAT_FILE_ATTRIBUTE_READ_ONLY (1 << 0)
#define EXFAT_FILE_ATTRIBUTE_HIDDEN    (1 << 1)
#define EXFAT_FILE_ATTRIBUTE_SYSTEM    (1 << 2)
#define EXFAT_FILE_ATTRIBUTE_DIRECTORY (1 << 4)
#define EXFAT_FILE_ATTRIBUTE_ARCHIVE   (1 << 5)

#define EXFAT_SECONDARY_ALLOCATION_POSSIBLE (1 << 0)
#define EXFAT_SECONDARY_NO_FAT_CHAIN        (1 << 1)

namespace exFAT
{
    #pragma pack(push, 1)
    typedef struct
    {
        uint8_t  type;         // 0x00
        uint8_t  bitmapFlags;  // 0x01
        uint8_t  unused[18];   // 0x02
        uint32_t firstCluster; // 0x14
        uint64_t length;       // 0x18
    } AllocationBitmapEntry; // 0x20

    typedef struct
    {
        uint8_t  type;          // 0x00
        uint8_t  unused1[3];    // 0x01
        uint32_t tableChecksum; // 0x04
        uint8_t  unused[12];    // 0x08
        uint32_t firstCluster;  // 0x14
        uint64_t length;        // 0x18
    } UpcaseTableEntry; // 0x20

    typedef struct
    {
        uint8_t  type;      // 0x00
        uint8_t  length;    // 0x01
        uint8_t  label[22]; // 0x02
        uint8_t  unused[8]; // 0x18
    } VolumeLabelEntry; // 0x20

    typedef struct
    {
        uint8_t  type;                      // 0x00
        uint8_t  secondaryCount;            // 0x01
        uint16_t setChecksum;               // 0x02
        uint16_t fileAttributes;            // 0x04
        uint8_t  unused1[2];                // 0x06
        uint32_t createdTimestamp;          // 0x08
        uint32_t lastModifiedTimestamp;     // 0x0c
        uint32_t lastAccessedTimestamp;     // 0x10
        uint8_t  create10msIncrement;       // 0x14
        uint8_t  lastModified10msIncrement; // 0x15
        uint8_t  createUtcOffset;           // 0x16
        uint8_t  lastModifiedUtcOffset;     // 0x17
        uint8_t  lastAccessedUtcOffset;     // 0x18
        uint8_t  unused2[7];                // 0x19
    } FileDirectoryEntry; // 0x20

    typedef struct
    {
        uint8_t  type;                // 0x00
        uint8_t  secondaryCount;      // 0x01
        uint16_t setChecksum;         // 0x02
        uint16_t generalPrimaryFlags; // 0x04
        uint8_t  guid[16];            // 0x06
        uint8_t  unused[10];          // 0x16
    } VolumeGUIDEntry; // 0x20

    typedef struct
    {
        uint8_t  type;                  // 0x00
        uint8_t  generalSecondaryFlags; // 0x01
        uint8_t  unused1;               // 0x02
        uint8_t  nameLength;            // 0x03
        uint16_t nameHash;              // 0x04
        uint8_t  unused2[2];            // 0x06
        uint64_t validDataLength;       // 0x08
        uint8_t  unused3[4];            // 0x10
        uint32_t firstCluster;          // 0x14
        uint64_t length;                // 0x18
    } StreamExtensionEntry; // 0x20

    typedef struct
    {
        uint8_t  type;                  // 0x00
        uint8_t  generalSecondaryFlags; // 0x01
        uint8_t  filename[30];          // 0x02
    } FilenameEntry; // 0x20

    class Entry
    {
    public:
        inline uint32_t getCluster()
        {
            return this->firstCluster;
        }

        inline uint32_t getLength()
        {
            return this->length;
        }

        inline uint8_t getType()
        {
            return this->type;
        }

        inline uint8_t getTypeCode()
        {
            return this->type & 0x1f;
        }

        inline uint8_t getTypeImportance()
        {
            return (this->type >> 5) & 1;
        }

        inline uint8_t getTypeCategory()
        {
            return (this->type >> 6) & 1;
        }

        inline uint8_t getInUse()
        {
            return (this->type >> 7) & 1;
        }

        inline bool isEndOfDirectory()
        {
            return this->type == 0x00;
        }

        inline bool isUnused()
        {
            return this->type >= 0x01 && this->type <= 0x7f;
        }
    private:
        uint8_t  type;         // 0x00
        uint8_t  unused[19];   // 0x01
        uint32_t firstCluster; // 0x14
        uint64_t length;       // 0x18
    }; // 0x20
    #pragma pack(pop)
};