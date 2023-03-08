#include <types.hpp>

#define SCSI_CMD_INQUIRY                               0x12
#define SCSI_CMD_REQUEST_SENSE                         0x03
#define SCSI_CMD_TEST_UNIT_READY                       0x00
#define SCSI_CMD_READ_CAPACITY_10                      0x25
#define SCSI_CMD_SEND_DIAGNOSTIC                       0x1D
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL          0x1E
#define SCSI_CMD_WRITE_10                              0x2A
#define SCSI_CMD_READ_10                               0x28
#define SCSI_CMD_WRITE_6                               0x0A
#define SCSI_CMD_READ_6                                0x08
#define SCSI_CMD_VERIFY_10                             0x2F
#define SCSI_CMD_MODE_SENSE_6                          0x1A
#define SCSI_CMD_MODE_SENSE_10                         0x5A

#define SCSI_SENSE_KEY_GOOD                            0x00
#define SCSI_SENSE_KEY_RECOVERED_ERROR                 0x01
#define SCSI_SENSE_KEY_NOT_READY                       0x02
#define SCSI_SENSE_KEY_MEDIUM_ERROR                    0x03
#define SCSI_SENSE_KEY_HARDWARE_ERROR                  0x04
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                 0x05
#define SCSI_SENSE_KEY_UNIT_ATTENTION                  0x06
#define SCSI_SENSE_KEY_DATA_PROTECT                    0x07
#define SCSI_SENSE_KEY_BLANK_CHECK                     0x08
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                 0x09
#define SCSI_SENSE_KEY_COPY_ABORTED                    0x0A
#define SCSI_SENSE_KEY_ABORTED_COMMAND                 0x0B
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                 0x0D
#define SCSI_SENSE_KEY_MISCOMPARE                      0x0E

#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION          0x00
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY             0x04
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB               0x24
#define SCSI_ASENSE_WRITE_PROTECTED                    0x27
#define SCSI_ASENSE_FORMAT_ERROR                       0x31
#define SCSI_ASENSE_INVALID_COMMAND                    0x20
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A

#define SCSI_ASENSEQ_NO_QUALIFIER                      0x00
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07

namespace SCSI
{
    typedef struct
    {
        uint32_t signature;           // Command block signature, always equal to CBW_SIGNATURE
        uint32_t tag;                 // Current CBW tag, to positively associate a CBW with a CSW (filled automatically)
        uint32_t dataTransferLength;  // Length of data to transfer, following the CBW
        uint8_t  flags;               // Block flags, equal to one of the COMMAND_DIRECTION_DATA_* macros
        uint8_t  lun;                 // Logical Unit Number the CBW is addressed to in the device
        uint8_t  scsiCommandLength;   // Length of the SCSI command in the CBW
        uint8_t  scsiCommandData[16]; // SCSI command to issue to the device
    } __attribute__ ((packed)) CommandBlock;

    typedef struct
    {
        uint32_t signature;           // Command status signature, always equal to CSW_SIGNATURE
        uint32_t tag;                 // Current CBW tag, to positively associate a CBW with a CSW
        uint32_t dataTransferResidue; // Length of data not transferred
        uint8_t  status;              // Command status
    } __attribute__ ((packed)) CommandStatus;

    typedef struct
    {
        uint32_t blocks;    // Number of blocks in the addressed LUN of the device
        uint32_t blockSize; // Number of bytes in each block in the addressed LUN
    } __attribute__ ((packed)) Capacity;

    typedef struct
    {
        uint8_t deviceType          : 5;
        uint8_t peripheralQualifier : 3;

        uint8_t _reserved1          : 7;
        uint8_t removable           : 1;

        uint8_t version;

        uint8_t responseDataFormat  : 4;
        uint8_t _reserved2          : 1;
        uint8_t normACA             : 1;
        uint8_t trmTsk              : 1;
        uint8_t aerc                : 1;

        uint8_t additionalLength;
        uint8_t _reserved3[2];

        uint8_t softReset           : 1;
        uint8_t cmdQue              : 1;
        uint8_t _reserved4          : 1;
        uint8_t linked              : 1;
        uint8_t sync                : 1;
        uint8_t wideBus16Bit        : 1;
        uint8_t wideBus32Bit        : 1;
        uint8_t relAddr             : 1;

        uint8_t vendorID[8];
        uint8_t productID[16];
        uint8_t revisionID[4];
    } __attribute__ ((packed)) Inquiry;
}