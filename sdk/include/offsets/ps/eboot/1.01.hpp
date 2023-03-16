#pragma once

#if ((defined(PS4) && PS4) || (defined(PS5) && PS5)) && defined(EBOOT_VERSION) && EBOOT_VERSION == 101
    // Handlers
    #define IO_REGISTER_READ_HANDLERS         0x060e7880
    #define INTERRUPT_WRITE_HANDLERS          0x00ae7d98
    #define IO_REGISTER_READ_HANDLER_ORIGINAL 0x005a9d60
    #define INTERRUPT_WRITE_HANDLER_ORIGINAL  0x0047da10

    // Threads
    #define EBOOT_THREAD_TROPHY_PUMP            0x00856348
    #define EBOOT_SEND_TROPHY_PUMP_COMMAND_FUNC 0x004e6c60

    // Game Pad Handles
    #define EBOOT_PADS                        0x1d26890

    // Mount Disc
    #define EBOOT_MOUNT_DISC_FUNC                   0x0460580
    #define EBOOT_MOUNT_DISC_OPTIONS_STACK_OFFSET   0x0412AA0 // 0x7EEFFAAA0 - 0x7EEBE8000
    #define EBOOT_MOUNT_DISC_GAME_CODE              0x0845a20

    // Configuration File
    #define EBOOT_PROCESS_CONFIG_FILE_FUNC          0x043cd70
    #define EBOOT_LOAD_LUA_SCRIPTS_FUNC             0x0465a70
    #define EBOOT_LUA_CONFIG_DIRECTORY              0x0856ed0
    #define EBOOT_LUA_TROPHY_DIRECTORY              0x0856f00
    #define EBOOT_LUA_FEATURE_DIRECTORY             0x0856f30
    #define EBOOT_LUA_TOOLING_DIRECTORY             0x0856f60
    #define EBOOT_LUA_LOCAL_CONFIG_FILE             0x0856fc0

    // Emu Command
    #define EBOOT_EMU_COMMAND                       0x0845a68
    #define EBOOT_EMU_COMMAND_COUNTER               0x0845a70

    // Function Stubs
    #define EBOOT_STRLEN_STUB                                 0x07635d0
    #define EBOOT_EXIT_STUB                                   0x0763630
    #define EBOOT_FEOF_STUB                                   0x0763640
    #define EBOOT_FREE_STUB                                   0x0763660
    #define EBOOT_FOPEN_S_STUB                                0x0763680
    #define EBOOT_FSEEK_STUB                                  0x0763690
    #define EBOOT_SCE_KERNEL_ALLOCATE_DIRECT_MEMORY_STUB      0x07636b0
    #define EBOOT_MALLOC_STATS_STUB                           0x07636e0
    #define EBOOT___ERROR_STUB                                0x07636f0
    #define EBOOT_MEMALIGN_STUB                               0x0763700
    #define EBOOT_SCE_KERNEL_MAP_DIRECT_MEMORY_STUB           0x0763710
    #define EBOOT_CALLOC_STUB                                 0x0763720
    #define EBOOT_PERROR_STUB                                 0x0763750
    #define EBOOT_STRNCPY_S_STUB                              0x0763770
    #define EBOOT_REALLOCALIGN_STUB                           0x0763780
    #define EBOOT_RENAME_STUB                                 0x07637a0
    #define EBOOT_REALLOC_STUB                                0x07637d0
    #define EBOOT_FGETC_STUB                                  0x0763800
    #define EBOOT_SCE_KERNEL_GET_DIRECT_MEMORY_SIZE_STUB      0x0763810
    #define EBOOT_SCE_KERNEL_CHECK_REACHABILITY_STUB          0x0763820
    #define EBOOT_SCE_KERNEL_FTRUNCATE_STUB                   0x0763830
    #define EBOOT_SCE_KERNEL_MKDIR_STUB                       0x0763840
    #define EBOOT_MALLOC_STUB                                 0x0763850
    #define EBOOT_STAT_STUB                                   0x0763860
    #define EBOOT_VSNPRINTF_STUB                              0x0763870
    #define EBOOT_VSPRINTF_S_STUB                             0x0763880
    #define EBOOT_STRERROR_S_STUB                             0x0763890
    #define EBOOT_MEMCHR_STUB                                 0x07638b0
    #define EBOOT_MEMCMP_STUB                                 0x07638d0
    #define EBOOT_SCE_KERNEL_WRITE_STUB                       0x07638e0
    #define EBOOT_SCE_KERNEL_PREAD_STUB                       0x07638f0
    #define EBOOT_SCE_KERNEL_OPEN_STUB                        0x0763900
    #define EBOOT_SCE_KERNEL_CLOSE_STUB                       0x0763910
    #define EBOOT_SCE_KERNEL_READ_STUB                        0x0763920
    #define EBOOT_SCE_APP_CONTENT_TEMPORARY_DATA_MOUNT_2_STUB 0x0763930
    #define EBOOT_SCE_KERNEL_STAT_STUB                        0x0763940
    #define EBOOT_WRITE_STUB                                  0x0763950
    #define EBOOT_SCE_KERNEL_LSEEK_STUB                       0x0763960
    #define EBOOT_SCE_KERNEL_PWRITE_STUB                      0x0763970
    #define EBOOT_READ_STUB                                   0x0763980
    #define EBOOT_MEMCPY_STUB                                 0x07645d0
    #define EBOOT_STRNCPY_STUB                                0x0763d20
    #define EBOOT_GETPID_STUB                                 0x07639c0
    #define EBOOT_SCE_PAD_SET_LIGHTBAR_STUB                   0x07641f0
    #define EBOOT_SCE_KERNEL_USLEEP_STUB                      0x0763b30

    #define EBOOT_SCE_COMMON_DIALOG_INITIALIZE_STUB 0x00763f00
    #define EBOOT_SCE_COMMON_DIALOG_IS_USED_STUB    0x00763ef0

    #define EBOOT_SCE_MSG_DIALOG_INITIALIZE_STUB    0x00763fa0
    #define EBOOT_SCE_MSG_DIALOG_OPEN_STUB          0x00764410
    #define EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB    0x00764030
    #define EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB 0x00764090
    #define EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB     0x00764450

    // Function Stub Pointers
    #define EBOOT_SCE_KERNEL_USLEEP_STUB_PTR            0x083d1c0
    #define EBOOT_SCE_KERNEL_REACHABILITY_STUB_PTR      0x083d038

    #define EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB_PTR    0x0083d440
    #define EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB_PTR 0x0083d470
    #define EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB_PTR     0x0083d650
#endif