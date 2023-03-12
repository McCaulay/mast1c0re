#pragma once

#if (defined(PS4) && PS4)
#include <types.hpp>
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL

// See: https://libusb.sourceforge.io/api-1.0/libusb_api.html

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

#define LIBUSB_CLASS_PER_INTERFACE       0
#define LIBUSB_CLASS_AUDIO               1
#define LIBUSB_CLASS_COMM                2
#define LIBUSB_CLASS_HID                 3
#define LIBUSB_CLASS_PHYSICAL            5
#define LIBUSB_CLASS_PRINTER             7
#define LIBUSB_CLASS_PTP                 6
#define LIBUSB_CLASS_IMAGE               6
#define LIBUSB_CLASS_MASS_STORAGE        8
#define LIBUSB_CLASS_HUB                 9
#define LIBUSB_CLASS_DATA                10
#define LIBUSB_CLASS_SMART_CARD          0x0b
#define LIBUSB_CLASS_CONTENT_SECURITY    0x0d
#define LIBUSB_CLASS_VIDEO               0x0e
#define LIBUSB_CLASS_PERSONAL_HEALTHCARE 0x0f
#define LIBUSB_CLASS_DIAGNOSTIC_DEVICE   0xdc
#define LIBUSB_CLASS_WIRELESS            0xe0
#define LIBUSB_CLASS_APPLICATION         0xfe
#define LIBUSB_CLASS_VENDOR_SPEC         0xff

#define LIBUSB_DT_DEVICE                0x01
#define LIBUSB_DT_CONFIG                0x02
#define LIBUSB_DT_STRING                0x03
#define LIBUSB_DT_INTERFACE             0x04
#define LIBUSB_DT_ENDPOINT              0x05
#define LIBUSB_DT_BOS                   0x0f
#define LIBUSB_DT_DEVICE_CAPABILITY     0x10
#define LIBUSB_DT_HID                   0x21
#define LIBUSB_DT_REPORT                0x22
#define LIBUSB_DT_PHYSICAL              0x23
#define LIBUSB_DT_HUB                   0x29
#define LIBUSB_DT_SUPERSPEED_HUB        0x2a
#define LIBUSB_DT_SS_ENDPOINT_COMPANION 0x30

#define LIBUSB_ENDPOINT_IN  0x80
#define LIBUSB_ENDPOINT_OUT 0x00

#define LIBUSB_TRANSFER_TYPE_MASK        0x03

#define LIBUSB_TRANSFER_TYPE_CONTROL     0
#define LIBUSB_TRANSFER_TYPE_ISOCHRONOUS 1
#define LIBUSB_TRANSFER_TYPE_BULK        2
#define LIBUSB_TRANSFER_TYPE_INTERRUPT   3
#define LIBUSB_TRANSFER_TYPE_BULK_STREAM 4

#define LIBUSB_REQUEST_TYPE_STANDARD (0x00 << 5)
#define LIBUSB_REQUEST_TYPE_CLASS    (0x01 << 5)
#define LIBUSB_REQUEST_TYPE_VENDOR   (0x02 << 5)
#define LIBUSB_REQUEST_TYPE_RESERVED (0x03 << 5)

#define LIBUSB_REQUEST_GET_STATUS        0x00
#define LIBUSB_REQUEST_CLEAR_FEATURE     0x01
#define LIBUSB_REQUEST_SET_FEATURE       0x03
#define LIBUSB_REQUEST_SET_ADDRESS       0x05
#define LIBUSB_REQUEST_GET_DESCRIPTOR    0x06
#define LIBUSB_REQUEST_SET_DESCRIPTOR    0x07
#define LIBUSB_REQUEST_GET_CONFIGURATION 0x08
#define LIBUSB_REQUEST_SET_CONFIGURATION 0x09
#define LIBUSB_REQUEST_GET_INTERFACE     0x0A
#define LIBUSB_REQUEST_SET_INTERFACE     0x0B
#define LIBUSB_REQUEST_SYNCH_FRAME       0x0C
#define LIBUSB_REQUEST_SET_SEL           0x30
#define LIBUSB_SET_ISOCH_DELAY           0x31

#define LIBUSB_RECIPIENT_DEVICE    0x00
#define LIBUSB_RECIPIENT_INTERFACE 0x01
#define LIBUSB_RECIPIENT_ENDPOINT  0x02
#define LIBUSB_RECIPIENT_OTHER     0x03

#define LIBUSB_SUCCESS             0
#define LIBUSB_ERROR_IO            -1
#define LIBUSB_ERROR_INVALID_PARAM -2
#define LIBUSB_ERROR_ACCESS        -3
#define LIBUSB_ERROR_NO_DEVICE     -4
#define LIBUSB_ERROR_NOT_FOUND     -5
#define LIBUSB_ERROR_BUSY          -6
#define LIBUSB_ERROR_TIMEOUT       -7
#define LIBUSB_ERROR_OVERFLOW      -8
#define LIBUSB_ERROR_PIPE          -9
#define LIBUSB_ERROR_INTERRUPTED   -10
#define LIBUSB_ERROR_NO_MEM        -11
#define LIBUSB_ERROR_NOT_SUPPORTED -12
#define LIBUSB_ERROR_OTHER         -99

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
                uint64_t /* unsigned char* */ extra;
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
                uint64_t /* PS::Sce::Usbd::EndpointDescriptor* */ endpoint;
                unsigned char *extra;
                int extraLength;
            } InterfaceDescriptor;

            typedef struct
            {
                uint64_t /* PS::Sce::Usbd::InterfaceDescriptor* */ altSetting;
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
                uint64_t /* PS::Sce::Usbd::Interface* */ interface;
                uint64_t /* unsigned char* */ extra;
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
            static void FreeConfigDescriptor(uint64_t /* Sce::Usbd::ConfigDescriptor* */ config);
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
            Usbd(uint16_t vendorId, uint16_t productId);
            bool Open();
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
            uint16_t vendorId;
            uint16_t productId;
        };
    }
}
#endif
#endif