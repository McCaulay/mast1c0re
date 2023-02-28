#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL

#define SCE_USBD_ERROR_ACCESS        0x80240003 // CE-30793-2
#define SCE_USBD_ERROR_BUSY          0x80240006 // CE-30796-5
#define SCE_USBD_ERROR_FATAL         0x802400ff // CE-30803-4
#define SCE_USBD_ERROR_INTERRUPTED   0x8024000a // CE-30800-1
#define SCE_USBD_ERROR_INVALID_ARG   0x80240002 // CE-30792-1
#define SCE_USBD_ERROR_IO            0x80240001 // CE-30791-0
#define SCE_USBD_ERROR_NO_DEVICE     0x80240004 // CE-30794-3
#define SCE_USBD_ERROR_NO_MEM        0x8024000b // CE-30801-2
#define SCE_USBD_ERROR_NOT_FOUND     0x80240005 // CE-30795-4
#define SCE_USBD_ERROR_NOT_SUPPORTED 0x8024000c // CE-30802-3
#define SCE_USBD_ERROR_OVERFLOW      0x80240008 // CE-30798-7
#define SCE_USBD_ERROR_PIPE          0x80240009 // CE-30799-8
#define SCE_USBD_ERROR_TIMEOUT       0x80240007 // CE-30797-6

#define SCE_USBD_RETRY_MAX       5
#define SCE_USBD_OUT_TIMEOUT     1000
#define SCE_USBD_IN_TIMEOUT      1000
#define SCE_USBD_CONTROL_TIMEOUT 1000

namespace PS
{
    namespace Sce
    {
        class Usbd
        {
        public:
            typedef struct
            {
                uint8_t length;
                uint8_t descriptorType;
                uint16_t bcdUSB;
                uint8_t deviceClass;
                uint8_t deviceSubClass;
                uint8_t deviceProtocol;
                uint8_t maxPacketSize0;
                uint16_t idVendor;
                uint16_t idProduct;
                uint16_t bcdDevice;
                uint8_t manufacturer;
                uint8_t product;
                uint8_t serialNumber;
                uint8_t numConfigurations;
            } DeviceDescriptor;

            typedef struct
            {
                uint8_t  length;
                uint8_t  descriptorType;
                uint8_t  endpointAddress;
                uint8_t  attributes;
                uint16_t maxPacketSize;
                uint8_t  interval;
                uint8_t  refresh;
                uint8_t  synchAddress;
                unsigned char *extra;
                int extraLength;
            } EndpointDescriptor;

            typedef struct
            {
                uint8_t length;
                uint8_t descriptorType;
                uint8_t interfaceNumber;
                uint8_t alternateSetting;
                uint8_t numEndpoints;
                uint8_t interfaceClass;
                uint8_t interfaceSubClass;
                uint8_t interfaceProtocol;
                uint8_t interface;
                PS::Sce::Usbd::EndpointDescriptor *endpoint;
                unsigned char *extra;
                int extraLength;
            } InterfaceDescriptor;

            typedef struct
            {
                PS::Sce::Usbd::InterfaceDescriptor *altSetting;
                int numAltSetting;
            } Interface;

            typedef struct
            {
                uint8_t  length;
                uint8_t  descriptorType;
                uint16_t totalLength;
                uint8_t  numInterfaces;
                uint8_t  configurationValue;
                uint8_t  configuration;
                uint8_t  attributes;
                uint8_t  maxPower;
                PS::Sce::Usbd::Interface *interface;
                unsigned char *extra;
                int extraLength;
            } ConfigDescriptor;
        public:
            static bool Initialize();
            static int32_t Init();
            static void Exit();
            static size_t GetDeviceList(uint64_t* list);
            static void FreeDeviceList(uint64_t list, int unrefDevices);
            static int32_t GetDeviceDescriptor(uint64_t device, PS::Sce::Usbd::DeviceDescriptor *desc);
            static int32_t Open(uint64_t device, uint64_t* handle);
            static uint64_t OpenDeviceWithVidPid(uint16_t vendorId, uint16_t productId);
            static int32_t GetActiveConfigDescriptor(uint64_t device, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config);
            static int32_t GetConfigDescriptor(uint64_t device, uint8_t configIndex, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config);
            static int32_t GetConfigDescriptorByValue(uint64_t device, uint8_t bConfigurationValue, uint64_t* /* Sce::Usbd::ConfigDescriptor** */ config);
            static void FreeConfigDescriptor(Sce::Usbd::ConfigDescriptor* config);
        private:
            static bool isInitialized;
            static int libUsbd;
            static uint64_t pInit;
            static uint64_t pExit;
            static uint64_t pGetDeviceList;
            static uint64_t pFreeDeviceList;
            static uint64_t pGetDeviceDescriptor;
            static uint64_t pOpen;
            static uint64_t pOpenDeviceWithVidPid;
            static uint64_t pClose;
            static uint64_t pSetInterfaceAltSetting;
            static uint64_t pClearHalt;
            static uint64_t pResetDevice;
            static uint64_t pCheckConnected;
            static uint64_t pControlTransfer;
            static uint64_t pBulkTransfer;
            static uint64_t pInterruptTransfer;
            static uint64_t pGetActiveConfigDescriptor;
            static uint64_t pGetConfigDescriptor;
            static uint64_t pGetConfigDescriptorByValue;
            static uint64_t pFreeConfigDescriptor;
        public:
            Usbd();
            Usbd(uint64_t device);
            Usbd(uint16_t vendorId, uint16_t productId);
            void Close();
            int32_t SetInterfaceAltSetting(int interfaceNumber, int alternateSetting);
            int32_t ClearHalt(uint8_t endpoint);
            int32_t ResetDevice();
            int32_t CheckConnected();
            int32_t ControlTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, uint8_t *data, uint16_t length, uint32_t timeout);
            int32_t BulkTransfer(uint8_t endpoint, uint8_t *data, int32_t length, int32_t *transferred, uint32_t timeout);
            int32_t InterruptTransfer(uint8_t endpoint, uint8_t *data, int32_t length, int32_t* transferred, uint32_t timeout);

            bool IsOpen();
            int32_t Send(uint8_t endpoint, uint8_t* data, int32_t length);
            int32_t Receive(uint8_t endpoint, uint8_t* data, int32_t length, int32_t* received);
        public:
            uint64_t handle;
        };
    }
}
#endif
#endif