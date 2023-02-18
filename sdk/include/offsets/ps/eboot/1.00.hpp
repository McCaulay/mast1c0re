#pragma once

#if ((defined(PS4) && PS4) || (defined(PS5) && PS5)) && defined(EBOOT_VERSION) && EBOOT_VERSION == 100
    // Game Pad Handles
    #define EBOOT_PADS                        0x3112520

    // Mount Disc
    #define EBOOT_MOUNT_DISC_FUNC                   0x0431cf0
    #define EBOOT_MOUNT_DISC_GAME_CODE              0x2d9adb8

    // Function Stubs
    #define EBOOT_SCE_MEMCPY_STUB                   0x074ffe0
    #define EBOOT_SCE_STRNCPY_STUB                  0x07504b0
    #define EBOOT_SCE_GETPID_STUB                   0x0750900
    #define EBOOT_SCE_PAD_SET_LIGHTBAR_STUB         0x0750c20
    #define EBOOT_SCE_KERNEL_USLEEP_STUB            0x0750420

    #define EBOOT_SCE_COMMON_DIALOG_INITIALIZE_STUB 0x07509e0

    #define EBOOT_SCE_MSG_DIALOG_INITIALIZE_STUB    0x07507d0
    #define EBOOT_SCE_MSG_DIALOG_OPEN_STUB          0x07507e0
    #define EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB    0x0750800
    #define EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB 0x07507f0
    #define EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB     0x0751290

    // Function Stub Pointers
    #define EBOOT_SCE_KERNEL_USLEEP_STUB_PTR            0x0813d18

    #define EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB_PTR    0x0813f08
    #define EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB_PTR 0x0813f00
    #define EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB_PTR     0x0814450
#endif