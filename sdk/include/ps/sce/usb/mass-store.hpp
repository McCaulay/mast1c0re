#pragma once

#if (defined(PS4) && PS4)
#include <types.hpp>
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/usbd.hpp>
#include <ps/sce/usb/scsi.hpp>

#define MASS_STORE_CBW_SIGNATURE 0x43425355UL // Command Block Wrapper signature byte, for verification of valid CBW blocks
#define MASS_STORE_CSW_SIGNATURE 0x53425355UL // Command Static Wrapper signature byte, for verification of valid CSW blocks

#define MASS_STORE_COMMAND_DIRECTION_DATA_OUT (0 << 7)     // Data direction mask for the Flags field of a CBW, indicating Host-to-Device transfer direction
#define MASS_STORE_COMMAND_DIRECTION_DATA_IN  (1 << 7)     // Data direction mask for the Flags field of a CBW, indicating Device-to-Host transfer direction
#define MASS_STORE_COMMAND_DATA_TIMEOUT_MS    10000        // Timeout period between the issuing of a CBW to a device, and the reception of the first packet
#define MASS_STORE_DATA_IN_PIPE               1            // Pipe number of the Mass Storage data IN pipe
#define MASS_STORE_DATA_OUT_PIPE              2            // Pipe number of the Mass Storage data OUT pipe

#define MASS_STORE_BULK_ONLY_DEFAULT_ENDPOINT_IN  0x81
#define MASS_STORE_BULK_ONLY_DEFAULT_ENDPOINT_OUT 0x02

#define MASS_STORE_BOMS_RESET       0xFF
#define MASS_STORE_BOMS_GET_MAX_LUN 0xFE

namespace PS
{
    class MassStore
    {
    public:
        MassStore();
        MassStore(PS::Sce::Usbd* usbd, uint8_t endpointIn = 0, uint8_t endpointOut = 0);

        uint8_t updateLUN();
        int32_t inquiry(SCSI::Inquiry* inquiry);
        int32_t readCapacity(SCSI::Capacity* capacity);
        int32_t readBlock(uint32_t blockAddress, uint16_t blocks, uint16_t blockSize, uint8_t* buffer);
    private:
        int32_t command(SCSI::CommandBlock* command, uint8_t* buffer);
        int32_t sendReceiveData(SCSI::CommandBlock* command, uint8_t* buffer);
        int32_t getReturnedStatus(SCSI::CommandStatus* status);
        uint8_t getEndpointIn();
        uint8_t getEndpointOut();
    private:
        static uint32_t tag;
    private:
        uint8_t lun;
        PS::Sce::Usbd* usbd;
        uint8_t endpointIn;
        uint8_t endpointOut;
    };
}
#endif
#endif