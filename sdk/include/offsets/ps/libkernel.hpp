#pragma once

// libkernel.sprx

#if defined(PS4) && PS4
    #if defined(FIRMWARE) && FIRMWARE == 505
        #define LIB_KERNEL_LEAKED                               true
        #define LIB_KERNEL_SCE_KERNEL_USLEEP                    0x013b20
        #define LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST 0x0191c0
        #define LIB_KERNEL_SCE_KERNEL_LOAD_START_MODULE         0x029e30
        #define LIB_KERNEL_SYS_RET                              0x002b9a
    #elif defined(FIRMWARE) && FIRMWARE == 900
        #define LIB_KERNEL_LEAKED                               true
        #define LIB_KERNEL_SCE_KERNEL_USLEEP                    0x014da0
        #define LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST 0x01a4e0
        #define LIB_KERNEL_SCE_KERNEL_LOAD_START_MODULE         0x02bbd0
        #define LIB_KERNEL_SYS_RET                              0x002baa
    #elif defined(FIRMWARE) && FIRMWARE == 1001
        #define LIB_KERNEL_LEAKED                               true
        #define LIB_KERNEL_SCE_KERNEL_USLEEP                    0x014a30
        #define LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST 0x01a170
        #define LIB_KERNEL_SCE_KERNEL_LOAD_START_MODULE         0x02b820
        #define LIB_KERNEL_SYS_RET                              0x002baa + 0x10 // +0x10 libkernel.sprx address
    #endif
#elif defined(PS5) && PS5
    #if defined(FIRMWARE) && FIRMWARE == 650
        #define LIB_KERNEL_LEAKED                               true
        #define LIB_KERNEL_SCE_KERNEL_USLEEP                    0x014f10
        #define LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST 0x01b570
        #define LIB_KERNEL_SCE_KERNEL_LOAD_START_MODULE         0x032470
        #define LIB_KERNEL_SYS_RET                              0x00038a
    #endif
#endif