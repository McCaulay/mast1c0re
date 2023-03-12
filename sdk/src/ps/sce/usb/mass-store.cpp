#if (defined(PS4) && PS4)
#include <ps/sce/usb/mass-store.hpp>
#ifdef LIBKERNEL
#include <ps/sce/sce.hpp>
#include <ps/sce/usb/usbd.hpp>
#include <ps/memory.hpp>

uint32_t PS::MassStore::tag = 1;

PS::MassStore::MassStore()
{
    this->usbd = nullptr;
    this->endpointIn = 0;
    this->endpointOut = 0;
    this->lun = 0;
}

PS::MassStore::MassStore(PS::Sce::Usbd* usbd, uint8_t endpointIn, uint8_t endpointOut)
{
    this->usbd = usbd;
    this->endpointIn = endpointIn;
    this->endpointOut = endpointOut;
    this->lun = 0;
}

uint8_t PS::MassStore::updateLUN()
{
    if (this->usbd == nullptr)
        return this->lun;

    int32_t err = this->usbd->ControlTransfer(MASS_STORE_COMMAND_DIRECTION_DATA_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, MASS_STORE_BOMS_GET_MAX_LUN, 0, 0, &this->lun, sizeof(lun), SCE_USBD_CONTROL_TIMEOUT);
    this->lun = err == SCE_OK ? lun : 0;
    return this->lun;
}

int32_t PS::MassStore::inquiry(SCSI::Inquiry* inquiry)
{
    SCSI::CommandBlock command = {
        .signature = MASS_STORE_CBW_SIGNATURE,
        .dataTransferLength = sizeof(SCSI::Inquiry),
        .flags = MASS_STORE_COMMAND_DIRECTION_DATA_IN,
        .lun = this->lun,
        .scsiCommandLength = 6,
        .scsiCommandData = { SCSI_CMD_INQUIRY, 0x00, 0x00, 0x00, sizeof(SCSI::Inquiry), 0x00 }
    };

    int32_t err = this->command(&command, (uint8_t*)inquiry);
    if (err != SCE_OK)
        return err;

    SCSI::CommandStatus status;
    return this->getReturnedStatus(&status);
}

int32_t PS::MassStore::readCapacity(SCSI::Capacity* capacity)
{
    SCSI::CommandBlock command = {
        .signature = MASS_STORE_CBW_SIGNATURE,
        .dataTransferLength = sizeof(SCSI::Capacity),
        .flags = MASS_STORE_COMMAND_DIRECTION_DATA_IN,
        .lun = this->lun,
        .scsiCommandLength = 10,
        .scsiCommandData = { SCSI_CMD_READ_CAPACITY_10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
    };

    int32_t err = this->command(&command, (uint8_t*)capacity);
    if (err != SCE_OK)
        return err;

    // Swap Endian
    capacity->blocks = PS::Memory::swapEndian(capacity->blocks);
    capacity->blockSize = PS::Memory::swapEndian(capacity->blockSize);

    SCSI::CommandStatus status;
    return this->getReturnedStatus(&status);
}

int32_t PS::MassStore::readBlock(uint32_t blockAddress, uint16_t blocks, uint16_t blockSize, uint8_t* buffer)
{
    SCSI::CommandBlock command = {
        .signature = MASS_STORE_CBW_SIGNATURE,
        .dataTransferLength = (uint32_t)blocks * (uint32_t)blockSize,
        .flags = MASS_STORE_COMMAND_DIRECTION_DATA_IN,
        .lun = this->lun,
        .scsiCommandLength = 10,
        .scsiCommandData = {
            SCSI_CMD_READ_10,
            0x00,
            (uint8_t)(blockAddress >> 24), // MSB of Block Address
            (uint8_t)(blockAddress >> 16),
            (uint8_t)(blockAddress >> 8),
            (uint8_t)(blockAddress & 0xFF),
            0x00,
            (uint8_t)(blocks >> 8),   // MSB Blocks to read
            (uint8_t)(blocks & 0xFF), // LSB Blocks to read
            0x00
        }
    };

    int32_t err = this->command(&command, buffer);
    if (err != SCE_OK)
    {
        PS::Debug.printf("PS::MassStore::readBlock: command error 0x%x\n", err);
        return err;
    }

    SCSI::CommandStatus status;
    return this->getReturnedStatus(&status);
}

int32_t PS::MassStore::command(SCSI::CommandBlock* command, uint8_t* buffer)
{
    if (this->usbd == nullptr)
        return SCE_USBD_ERROR_NO_DEVICE;

    // Set unique tag
    command->tag = PS::MassStore::tag;
    PS::MassStore::tag++;

    // Send data
    int32_t err = this->usbd->Send(this->getEndpointOut(), (uint8_t*)command, sizeof(SCSI::CommandBlock));
    if (err != SCE_OK)
        return err;

    // Receive data
    if (buffer != nullptr)
        return this->sendReceiveData(command, buffer);
    return SCE_OK;
}

int32_t PS::MassStore::sendReceiveData(SCSI::CommandBlock* command, uint8_t* buffer)
{
    if (this->usbd == nullptr)
        return SCE_USBD_ERROR_NO_DEVICE;

    uint16_t dataTransferLength = command->dataTransferLength;

    // Receive
    if (command->flags & MASS_STORE_COMMAND_DIRECTION_DATA_IN)
    {
        int32_t received = 0;
        return this->usbd->Receive(this->getEndpointIn(), buffer, dataTransferLength, &received);
    }

    // Send
    return this->usbd->Send(this->getEndpointOut(), buffer, dataTransferLength);
}

int32_t PS::MassStore::getReturnedStatus(SCSI::CommandStatus* status)
{
    if (this->usbd == nullptr)
        return SCE_USBD_ERROR_NO_DEVICE;

    int32_t received = 0;
    return this->usbd->Receive(this->getEndpointIn(), (uint8_t*)status, sizeof(SCSI::CommandStatus), &received);
}

uint8_t PS::MassStore::getEndpointIn()
{
    return this->endpointIn != 0 ? this->endpointIn : MASS_STORE_BULK_ONLY_DEFAULT_ENDPOINT_IN;
}

uint8_t PS::MassStore::getEndpointOut()
{
    return this->endpointOut != 0 ? this->endpointOut : MASS_STORE_BULK_ONLY_DEFAULT_ENDPOINT_OUT;
}
#endif
#endif