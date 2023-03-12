#if (defined(PS4) && PS4)
#include <ps/sce/usb/usb/usb.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/usbd.hpp>
#include <ps/memory.hpp>
#include <ps2/ps2.hpp>
#include <ps/sce/sce.hpp>
#include <ps/sce/usb/filesystems/exfat/boot-sector.hpp>

uint64_t Usb::devices = 0;

List<Usb> Usb::list()
{
    // Free any previous allocations
    if (Usb::devices != 0)
    {
        PS::Sce::Usbd::FreeDeviceList(Usb::devices, 1);
        Usb::devices = 0;
    }

    List<Usb> usbs = List<Usb>();

    uint32_t deviceCount = (uint32_t)PS::Sce::Usbd::GetDeviceList(&Usb::devices);
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        uint64_t device = PS::Memory::read<uint64_t>(Usb::devices, i);
        usbs.add(Usb(device));
    }
    return usbs;
}

Usb::Usb()
{
    this->device = 0;
    this->vid = 0;
    this->pid = 0;
    this->endpointIn = 0;
    this->endpointOut = 0;
    this->endpointInMaxPacket = 0;
    this->endpointOutMaxPacket = 0;
    this->usbd = PS::Sce::Usbd();
    this->massStore = PS::MassStore(&this->usbd);
    this->mounted = false;
    this->filesystem = exFAT::Filesystem();

    PS2::memset(this->vendor, 0x00, sizeof(this->vendor));
    PS2::memset(this->product, 0x00, sizeof(this->product));

    this->sectorCount = 0;
    this->sectorSize = 0;
}

Usb::Usb(uint64_t device)
{
    this->device = device;
    this->vid = 0;
    this->pid = 0;
    this->endpointIn = 0;
    this->endpointOut = 0;
    this->endpointInMaxPacket = 0;
    this->endpointOutMaxPacket = 0;
    this->usbd = PS::Sce::Usbd();
    this->massStore = PS::MassStore(&this->usbd);
    this->mounted = false;
    this->filesystem = exFAT::Filesystem();

    PS2::memset(this->vendor, 0x00, sizeof(this->vendor));
    PS2::memset(this->product, 0x00, sizeof(this->product));

    this->sectorCount = 0;
    this->sectorSize = 0;
}

Usb::Usb(uint16_t vid, uint16_t pid)
{
    this->device = 0;
    this->vid = vid;
    this->pid = pid;
    this->endpointIn = 0;
    this->endpointOut = 0;
    this->endpointInMaxPacket = 0;
    this->endpointOutMaxPacket = 0;
    this->usbd = PS::Sce::Usbd(this->vid, this->pid);
    this->massStore = PS::MassStore(&this->usbd);
    this->mounted = false;
    this->filesystem = exFAT::Filesystem();

    PS2::memset(this->vendor, 0x00, sizeof(this->vendor));
    PS2::memset(this->product, 0x00, sizeof(this->product));

    this->sectorCount = 0;
    this->sectorSize = 0;
}

bool Usb::isOpen()
{
    return this->usbd.IsOpen();
}

bool Usb::open()
{
    if (this->vid == 0 && this->pid == 0)
    {
        if (!this->updateDescriptors())
            return false;
    }

    if (this->isOpen())
        return true;

    return this->usbd.Open();
}

bool Usb::updateDescriptors()
{
    if (this->device == 0)
        return false;

    PS::Sce::Usbd::DeviceDescriptor descriptor;
    if (PS::Sce::Usbd::GetDeviceDescriptor(this->device, &descriptor) != SCE_OK)
        return false;

    if (descriptor.descriptorType != LIBUSB_DT_DEVICE)
        return false;

    PS::Debug.printf("Device #%x\n", this->device);
    PS::Debug.printf("  Type:          0x%02x\n", descriptor.descriptorType);
    PS::Debug.printf("  Vendor Id:     0x%04x\n", descriptor.idVendor);
    PS::Debug.printf("  Product Id:    0x%04x\n", descriptor.idProduct);
    PS::Debug.printf("  Class:         0x%02x\n", descriptor.deviceClass);
    PS::Debug.printf("  Sub Class:     0x%02x\n", descriptor.deviceSubClass);
    PS::Debug.printf("  Protocol:      0x%02x\n", descriptor.deviceProtocol);
    PS::Debug.printf("  Manufacturer:  0x%02x\n", descriptor.manufacturer);
    PS::Debug.printf("  Product:       0x%02x\n", descriptor.product);
    PS::Debug.printf("  Serial Number: 0x%02x\n", descriptor.serialNumber);

    bool isMassStorage = false;

    // Loop configurations
    PS::Debug.printf("  Configurations (%i)\n", descriptor.numConfigurations);
    for (uint32_t i = 0; i < descriptor.numConfigurations; i++)
    {
        // Get config descriptor
        uint64_t configPtr = 0;
        if (PS::Sce::Usbd::GetConfigDescriptor(this->device, (uint8_t)i, &configPtr) != SCE_OK)
            continue;

        // Dereference config descriptor pointer
        PS::Sce::Usbd::ConfigDescriptor config = PS::Memory::read<PS::Sce::Usbd::ConfigDescriptor>(configPtr);

        if (config.descriptorType != LIBUSB_DT_CONFIG)
            continue;

        PS::Debug.printf("    Config #%i\n", i + 1);
        PS::Debug.printf("      Type:          0x%02x\n", config.descriptorType);
        PS::Debug.printf("      Value:         0x%02x\n", config.configurationValue);
        PS::Debug.printf("      Configuration: 0x%02x\n", config.configuration);
        PS::Debug.printf("      Attributes:    0x%02x\n", config.attributes);
        PS::Debug.printf("      Max Power:     0x%02x\n", config.maxPower);

        // Loop interfaces
        PS::Debug.printf("      Interfaces (%i)\n", config.numInterfaces);
        for (uint32_t j = 0; j < config.numInterfaces; j++)
        {
            PS::Sce::Usbd::Interface interface = PS::Memory::read<PS::Sce::Usbd::Interface>(config.interface, j);

            PS::Debug.printf("        Interface #%i\n", j + 1);

            // Loop interface settings
            PS::Debug.printf("          Alternate Settings (%i)\n", config.numInterfaces);
            for (uint32_t k = 0; k < interface.numAltSetting; k++)
            {
                PS::Sce::Usbd::InterfaceDescriptor interfaceDescriptor = PS::Memory::read<PS::Sce::Usbd::InterfaceDescriptor>(interface.altSetting, k);

                PS::Debug.printf("            Alternate Setting: #%i\n", k + 1);
                PS::Debug.printf("              Number:    %i\n", interfaceDescriptor.interfaceNumber);
                PS::Debug.printf("              Type:      0x%02x\n", interfaceDescriptor.descriptorType);
                PS::Debug.printf("              Setting:   0x%02x\n", interfaceDescriptor.alternateSetting);
                PS::Debug.printf("              Class:     0x%02x\n", interfaceDescriptor.interfaceClass);
                PS::Debug.printf("              Sub Class: 0x%02x\n", interfaceDescriptor.interfaceSubClass);
                PS::Debug.printf("              Protocol:  0x%02x\n", interfaceDescriptor.interfaceProtocol);
                PS::Debug.printf("              Interface: 0x%02x\n", interfaceDescriptor.interface);

                // Check interface is mass storage
                if (interfaceDescriptor.interfaceClass == LIBUSB_CLASS_MASS_STORAGE &&
                    (interfaceDescriptor.interfaceSubClass == 0x01 || interfaceDescriptor.interfaceSubClass == 0x06) &&
                    interfaceDescriptor.interfaceProtocol == 0x50)
                    isMassStorage = true;

                // Loop endpoints
                PS::Debug.printf("              Endpoints (%i)\n", interfaceDescriptor.numEndpoints);
                for (uint32_t l = 0; l < interfaceDescriptor.numEndpoints; l++)
                {
                    PS::Sce::Usbd::EndpointDescriptor endpoint = PS::Memory::read<PS::Sce::Usbd::EndpointDescriptor>(interfaceDescriptor.endpoint, l);
                    PS::Debug.printf("                Endpoint: #%i\n", l + 1);
                    PS::Debug.printf("                  Type:       0x%02x\n", endpoint.descriptorType);
                    PS::Debug.printf("                  Address:    0x%02x\n", endpoint.endpointAddress);
                    PS::Debug.printf("                  Attributes: 0x%02x\n", endpoint.attributes);
                    PS::Debug.printf("                  Max Packet Size: 0x%04x\n", endpoint.maxPacketSize);
                    PS::Debug.printf("                  Polling Interval: 0x%02x\n", endpoint.interval);
                    PS::Debug.printf("                  Refresh: 0x%02x\n", endpoint.refresh);

                    if ((endpoint.attributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_BULK | LIBUSB_TRANSFER_TYPE_INTERRUPT))
                    {
                        if (endpoint.endpointAddress & LIBUSB_ENDPOINT_IN)
                        {
                            this->endpointIn = this->endpointIn ? this->endpointIn : endpoint.endpointAddress;
                            this->endpointInMaxPacket = this->endpointInMaxPacket ? this->endpointInMaxPacket : endpoint.maxPacketSize;
                        }
                        else
                        {
                            this->endpointOut = this->endpointOut ? this->endpointOut : endpoint.endpointAddress;
                            this->endpointOutMaxPacket = this->endpointOutMaxPacket ? this->endpointOutMaxPacket : endpoint.maxPacketSize;
                        }
                    }
                }
            }
        }

        // Free config descriptor
        PS::Sce::Usbd::FreeConfigDescriptor(configPtr);
    }

    if (!isMassStorage)
        return false;

    this->vid = descriptor.idVendor;
    this->pid = descriptor.idProduct;

    if (this->usbd.IsOpen())
        this->usbd.Close();

    this->usbd = PS::Sce::Usbd(this->vid, this->pid);
    this->massStore = PS::MassStore(&this->usbd, this->endpointIn, this->endpointOut);
    return true;
}

bool Usb::mount()
{
    if (this->isMounted())
        return true;

    if (!this->isOpen() && !this->open())
        return false;

    PS::Debug.printf("Mounting USB: VID=%04x, PID=%04x\n", this->vid, this->pid);

    // Get max logical unit number (LUN)
    uint8_t lun = this->massStore.updateLUN();
    PS::Debug.printf("LUN: %i\n", lun);

    SCSI::Inquiry inquiry;
    if (this->massStore.inquiry(&inquiry) != SCE_OK)
        return false;

    PS2::strcpy(this->vendor, (const char*)inquiry.vendorID);
    this->vendor[sizeof(this->vendor) - 1] = '\0';
    PS2::strcpy(this->product, (const char*)inquiry.productID);
    this->product[sizeof(this->product) - 1] = '\0';

    PS::Debug.printf("Vendor: %s\n", this->vendor);
    PS::Debug.printf("Product: %s\n", this->product);

    // Get capacity
    SCSI::Capacity capacity;
    if (this->massStore.readCapacity(&capacity) != SCE_OK)
        return false;

    this->sectorCount = capacity.blocks;
    this->sectorSize = capacity.blockSize;

    PS::Debug.printf("Sector Count: 0x%x\n", this->sectorCount);
    PS::Debug.printf("Sector Size: %u\n", this->sectorSize);

    // Only support fixed sector size
    if (this->sectorSize != SECTOR_SIZE)
        return false;

    // Find filesystem
    if (!this->findFilesystem())
    {
        PS::Debug.printf("Failed to find exFAT filesystem\n");
        return false;
    }

    // Mount filesystem
    this->mounted = this->filesystem.mount();
    return this->mounted;
}

void Usb::unmount()
{
    if (!this->isMounted())
        return;
    this->mounted = false;
}

bool Usb::isMounted()
{
    return this->mounted;
}

void Usb::close()
{
    if (this->usbd.IsOpen())
        this->usbd.Close();
}

uint32_t Usb::getClusterSize()
{
    if (!this->isMounted())
        return 0;

    return this->filesystem.getClusterSize();
}

bool Usb::root(List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.root(directories, files);
}

bool Usb::directory(const char* filepath, List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.directory(filepath, directories, files);
}

bool Usb::directory(exFAT::Directory parent, List<exFAT::Directory>* directories, List<exFAT::File>* files)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.directory(parent, directories, files);
}

List<exFAT::Directory> Usb::directories(const char* filepath)
{
    if (!this->isMounted())
        return List<exFAT::Directory>();

    return this->filesystem.directories(filepath);
}

List<exFAT::Directory> Usb::directories(exFAT::Directory parent)
{
    if (!this->isMounted())
        return List<exFAT::Directory>();

    return this->filesystem.directories(parent);
}

List<exFAT::File> Usb::files(const char* filepath)
{
    if (!this->isMounted())
        return List<exFAT::File>();

    return this->filesystem.files(filepath);
}

List<exFAT::File> Usb::files(exFAT::Directory parent)
{
    if (!this->isMounted())
        return List<exFAT::File>();

    return this->filesystem.files(parent);
}

bool Usb::exists(const char* filepath)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.exists(filepath);
}

bool Usb::resetRead(exFAT::File file)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.resetRead(file);
}

bool Usb::readNextCluster(exFAT::File file, uint8_t* buffer)
{
    if (!this->isMounted())
        return false;

    return this->filesystem.readNextCluster(file, buffer);
}

bool Usb::readSector(uint32_t blockAddress, uint8_t* buffer)
{
    return this->readSectors(blockAddress, buffer, 1);
}

bool Usb::readSectors(uint32_t blockAddress, uint8_t* buffer, uint32_t count)
{
    int error = this->massStore.readBlock(blockAddress, count, this->sectorSize, buffer);
    if (error != SCE_OK)
        PS::Debug.printf("Mass Store read block failed with error 0x%x\n", error);
    return error == SCE_OK;
}

bool Usb::isSectorExFAT(uint8_t* sector)
{
    // https://en.wikipedia.org/wiki/BIOS_parameter_block

    // exFAT
    return PS2::memcmp(sector + 0x03, (void*)"EXFAT   ", 8) == 0;
}

bool Usb::findFilesystem()
{
    // Read boot sector
    uint8_t bootSector[SECTOR_SIZE];
    if (!this->readSector(0, bootSector))
    {
        PS::Debug.printf("Failed to read boot sector\n");
        return false;
    }

    // exFAT is in boot sector with no partition table
    if (this->isSectorExFAT(bootSector))
    {
        this->filesystem = exFAT::Filesystem(&this->massStore, 0, *(exFAT::BootSector*)bootSector);
        return true;
    }

    // Read boot sector as master boot record table
    MasterBootRecordTable mbr = *(MasterBootRecordTable*)bootSector;

    // Validate master boot record
    if (!mbr.isValid())
    {
        PS::Debug.printf("Master boot record is invalid\n");
        return false;
    }

    // Loop each partition
    for (uint32_t i = 0; i < mbr.getMaxParitionCount(); i++)
    {
        MasterBootRecord partition = mbr.getParition(i);
        if (partition.isEmpty())
            continue;

        // Determine partition filesystem type
        if (!this->readSector(partition.logicalBlockAddress, bootSector))
            continue;

        // exFAT check
        if (this->isSectorExFAT(bootSector))
        {
            this->partition = partition;
            this->filesystem = exFAT::Filesystem(&this->massStore, this->partition.logicalBlockAddress, *(exFAT::BootSector*)bootSector);
            return true;
        }

        // Unhandled filesystem
        continue;
    }

    // Failed to find exFAT filesystem
    return false;
}
#endif
#endif