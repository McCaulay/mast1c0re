#if (defined(PS4) && PS4)
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/sce.hpp>
#include <ps/sce/usb/usbd.hpp>

bool PS::Sce::Usbd::isInitialized = false;
int PS::Sce::Usbd::libUsbd = 0;
uint64_t PS::Sce::Usbd::pInit = 0;
uint64_t PS::Sce::Usbd::pExit = 0;
uint64_t PS::Sce::Usbd::pGetDeviceList = 0;
uint64_t PS::Sce::Usbd::pFreeDeviceList = 0;
uint64_t PS::Sce::Usbd::pGetDeviceDescriptor = 0;
uint64_t PS::Sce::Usbd::pOpen = 0;
uint64_t PS::Sce::Usbd::pOpenDeviceWithVidPid = 0;
uint64_t PS::Sce::Usbd::pClose = 0;
uint64_t PS::Sce::Usbd::pSetInterfaceAltSetting = 0;
uint64_t PS::Sce::Usbd::pClearHalt = 0;
uint64_t PS::Sce::Usbd::pResetDevice = 0;
uint64_t PS::Sce::Usbd::pCheckConnected = 0;
uint64_t PS::Sce::Usbd::pControlTransfer = 0;
uint64_t PS::Sce::Usbd::pBulkTransfer = 0;
uint64_t PS::Sce::Usbd::pInterruptTransfer = 0;
uint64_t PS::Sce::Usbd::pGetActiveConfigDescriptor = 0;
uint64_t PS::Sce::Usbd::pGetConfigDescriptor = 0;
uint64_t PS::Sce::Usbd::pGetConfigDescriptorByValue = 0;
uint64_t PS::Sce::Usbd::pFreeConfigDescriptor = 0;

bool PS::Sce::Usbd::Initialize()
{
    if (PS::Sce::Usbd::isInitialized)
        return true;

    // Load libSceUsbd
    PS::Sce::Usbd::libUsbd = PS::Sce::Kernel::LoadStartModule("libSceUsbd.sprx", 0, NULL, 0, 0, 0);
    if (PS::Sce::Usbd::libUsbd < 0)
        return false;

    #if defined(PS4) && PS4
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdInit", &PS::Sce::Usbd::pInit);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdExit", &PS::Sce::Usbd::pExit);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdGetDeviceList", &PS::Sce::Usbd::pGetDeviceList);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdFreeDeviceList", &PS::Sce::Usbd::pFreeDeviceList);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdGetDeviceDescriptor", &PS::Sce::Usbd::pGetDeviceDescriptor);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdOpen", &PS::Sce::Usbd::pOpen);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdOpenDeviceWithVidPid", &PS::Sce::Usbd::pOpenDeviceWithVidPid);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdClose", &PS::Sce::Usbd::pClose);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdSetInterfaceAltSetting", &PS::Sce::Usbd::pSetInterfaceAltSetting);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdClearHalt", &PS::Sce::Usbd::pClearHalt);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdResetDevice", &PS::Sce::Usbd::pResetDevice);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdCheckConnected", &PS::Sce::Usbd::pCheckConnected);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdControlTransfer", &PS::Sce::Usbd::pControlTransfer);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdBulkTransfer", &PS::Sce::Usbd::pBulkTransfer);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdInterruptTransfer", &PS::Sce::Usbd::pInterruptTransfer);

    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdGetActiveConfigDescriptor", &PS::Sce::Usbd::pGetActiveConfigDescriptor);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdGetConfigDescriptor", &PS::Sce::Usbd::pGetConfigDescriptor);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdGetConfigDescriptorByValue", &PS::Sce::Usbd::pGetConfigDescriptorByValue);
    PS::Sce::Kernel::Dlsym(PS::Sce::Usbd::libUsbd, "sceUsbdFreeConfigDescriptor", &PS::Sce::Usbd::pFreeConfigDescriptor);
    #endif

    PS::Sce::Usbd::Init();
    PS::Sce::Usbd::isInitialized = true;
    return true;
}

int32_t PS::Sce::Usbd::Init()
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pInit);
}

void PS::Sce::Usbd::Exit()
{
    PS::Sce::Usbd::Initialize();
    PS::Breakout::call(PS::Sce::Usbd::pExit);
}

size_t PS::Sce::Usbd::GetDeviceList(uint64_t* list)
{
    PS::Sce::Usbd::Initialize();
    return (size_t)PS::Breakout::call(PS::Sce::Usbd::pGetDeviceList, PVAR_TO_NATIVE(list));
}

void PS::Sce::Usbd::FreeDeviceList(uint64_t list, int unrefDevices)
{
    PS::Sce::Usbd::Initialize();
    PS::Breakout::call(PS::Sce::Usbd::pFreeDeviceList, list, unrefDevices);
}

int32_t PS::Sce::Usbd::GetDeviceDescriptor(uint64_t device, PS::Sce::Usbd::DeviceDescriptor *desc)
{
    PS::Sce::Usbd::Initialize();
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pGetDeviceDescriptor, device, PVAR_TO_NATIVE(desc));
}

uint64_t PS::Sce::Usbd::OpenDeviceWithVidPid(uint16_t vendorId, uint16_t productId)
{
    PS::Sce::Usbd::Initialize();
    return (uint64_t)PS::Breakout::call(PS::Sce::Usbd::pOpenDeviceWithVidPid, (uint64_t)vendorId, (uint64_t)productId);
}

int32_t PS::Sce::Usbd::GetActiveConfigDescriptor(uint64_t device, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config)
{
    PS::Sce::Usbd::Initialize();
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pGetActiveConfigDescriptor, device, PVAR_TO_NATIVE(config));
}

int32_t PS::Sce::Usbd::GetConfigDescriptor(uint64_t device, uint8_t configIndex, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config)
{
    PS::Sce::Usbd::Initialize();
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pGetConfigDescriptor, device, configIndex, PVAR_TO_NATIVE(config));
}

int32_t PS::Sce::Usbd::GetConfigDescriptorByValue(uint64_t device, uint8_t bConfigurationValue, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config)
{
    PS::Sce::Usbd::Initialize();
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pGetConfigDescriptorByValue, device, bConfigurationValue, PVAR_TO_NATIVE(config));
}

void PS::Sce::Usbd::FreeConfigDescriptor(uint64_t /* Sce::Usbd::ConfigDescriptor* */ config)
{
    PS::Sce::Usbd::Initialize();
    PS::Breakout::call(PS::Sce::Usbd::pFreeConfigDescriptor, config);
}

PS::Sce::Usbd::Usbd()
{
    this->handle = 0;
    this->vendorId = 0;
    this->productId = 0;
}

PS::Sce::Usbd::Usbd(uint16_t vendorId, uint16_t productId)
{
    this->handle = 0;
    this->vendorId = vendorId;
    this->productId = productId;
}

bool PS::Sce::Usbd::Open()
{
    if (this->vendorId == 0 && this->productId == 0)
        return false;

    PS::Sce::Usbd::Initialize();
    if (this->IsOpen())
        return true;
    this->handle = PS::Sce::Usbd::OpenDeviceWithVidPid(this->vendorId, this->productId);
    return this->IsOpen();
}

void PS::Sce::Usbd::Close()
{
    PS::Breakout::call(PS::Sce::Usbd::pClose, this->handle);
    this->handle = 0;
}

int32_t PS::Sce::Usbd::SetInterfaceAltSetting(int interfaceNumber, int alternateSetting)
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pSetInterfaceAltSetting, this->handle, interfaceNumber, alternateSetting);
}

int32_t PS::Sce::Usbd::ClearHalt(uint8_t endpoint)
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pClearHalt, this->handle, endpoint);
}

int32_t PS::Sce::Usbd::ResetDevice()
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pResetDevice, this->handle);
}

int32_t PS::Sce::Usbd::CheckConnected()
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pCheckConnected, this->handle);
}

int32_t PS::Sce::Usbd::ControlTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, uint8_t *data, uint16_t length, uint32_t timeout)
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pControlTransfer, this->handle, requestType, request, value, index, PVAR_TO_NATIVE(data), length, timeout);
}

int32_t PS::Sce::Usbd::BulkTransfer(uint8_t endpoint, uint8_t *data, int32_t length, int32_t *transferred, uint32_t timeout)
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pBulkTransfer, this->handle, endpoint, PVAR_TO_NATIVE(data), length, PVAR_TO_NATIVE(transferred), timeout);
}

int32_t PS::Sce::Usbd::InterruptTransfer(uint8_t endpoint, uint8_t *data, int32_t length, int32_t* transferred, uint32_t timeout)
{
    return (int32_t)PS::Breakout::call(PS::Sce::Usbd::pInterruptTransfer, this->handle, endpoint, PVAR_TO_NATIVE(data), length, PVAR_TO_NATIVE(transferred), timeout);
}

bool PS::Sce::Usbd::IsOpen()
{
    return this->handle != 0;
}

int32_t PS::Sce::Usbd::Send(uint8_t endpoint, uint8_t* data, int32_t length)
{
    int32_t transferred = 0;
    int err = SCE_OK;
    for (int i = 0; i < SCE_USBD_RETRY_MAX; i++)
    {
        err = this->BulkTransfer(endpoint, data, length, &transferred, SCE_USBD_OUT_TIMEOUT);
        if (err != SCE_USBD_ERROR_PIPE)
            break;
        this->ClearHalt(endpoint);
    }
    return err;
}

int32_t PS::Sce::Usbd::Receive(uint8_t endpoint, uint8_t* data, int32_t length, int32_t* received)
{
    int transferred = 0;
    int err = SCE_OK;
    for (int i = 0; i < SCE_USBD_RETRY_MAX; i++)
    {
        err = this->BulkTransfer(endpoint, data, length, received, SCE_USBD_IN_TIMEOUT);
        if (err != SCE_USBD_ERROR_PIPE)
            break;
        this->ClearHalt(endpoint);
    }
    return err;
}
#endif
#endif