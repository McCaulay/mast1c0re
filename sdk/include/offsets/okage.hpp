#pragma once

// common
#define PS2_MALLOC                    0x001bda38
#define PS2_FREE                      0x001bdaf0
#define PS2_STRNCPY                   0x001f1e60
#define PS2_STRCMP                    0x001f1918
#define PS2_STRLEN                    0x001f1b70
#define PS2_MEMCPY                    0x001f131c
#define PS2_SPRINTF                   0x001f15a8
#define PS2_VSPRINTF                  0x001f4b08

// syscalls
#define PS2_SYS_RESET_EE              0x001e4010
#define PS2_SYS_LOAD_EXEC_PS2         0x001e4060
#define PS2_SYS_EXEC_PS2              0x001e4070
#define PS2_SYS_GET_THREAD_ID         0x001e4310
#define PS2_SYS_CREATE_THREAD         0x001e4220
#define PS2_SYS_START_THREAD          0x001e4240
#define PS2_SYS_JOIN_THREAD           0x001e43d0
#define PS2_SYS_SLEEP_THREAD          0x001e4390
#define PS2_SYS_EXIT_DELETE_THREAD    0x001e4260
#define PS2_SYS_SUSPEND_THREAD        0x001e4340
#define PS2_SYS_TERMINATE_THREAD      0x001e4270
#define PS2_SYS_DELETE_THREAD         0x001e4230
#define PS2_SYS_EXIT                  0x001e4040

// Sif
#define PS2_SIF_IOP_RESET             0x001e8da0
#define PS2_SIF_IOP_SYNC              0x001e8ef8
#define PS2_SIF_INIT_RPC              0x001e6630
#define PS2_SIF_EXIT_RPC              0x001e67f0

// libmc
#define PS2_MC_INIT                   0x001d7790
#define PS2_MC_GET_INFO               0x001d8050
#define PS2_MC_OPEN                   0x001d7a28
#define PS2_MC_CLOSE                  0x001d7b48
#define PS2_MC_SEEK                   0x001d7bd8
#define PS2_MC_READ                   0x001d7cf8
#define PS2_MC_WRITE                  0x001d7dd0
#define PS2_MC_FLUSH                  0x001d8548
#define PS2_MC_MK_DIR                 0x001d7b10
#define PS2_MC_CH_DIR                 0x001d82f8
#define PS2_MC_GET_DIR                0x001d8188
#define PS2_MC_SET_FILE_INFO          0x001d85d8
#define PS2_MC_DELETE                 0x001d8470
#define PS2_MC_FORMAT                 0x001d83e0
#define PS2_MC_UNFORMAT               0x001d8830
#define PS2_MC_GET_ENT_SPACE          0x001d88c0
#define PS2_MC_RENAME                 0x001d8730
#define PS2_MC_CHANGE_THREAD_PRIORITY 0x001d7910
#define PS2_MC_SYNC                   0x001d7f10

// Okage
#define OKAGE_MEMORY_CARD_OPEN        0x0016c778
#define OKAGE_MEMORY_CARD_READ        0x0016c8b0
#define OKAGE_MEMORY_CARD_WRITE       0x0016c920
#define OKAGE_MEMORY_CARD_SEEK        0x0016c840
#define OKAGE_MEMORY_CARD_CLOSE       0x0016c7f0

// Okage (System Dynamic)
#if (defined(PCSX2) && PCSX2)
    #define OKAGE_SCREEN_LOAD_NAME     0x008B8C40
    #define OKAGE_SCREEN_LOAD_LOCATION 0x008B99C0
#elif ((defined(PS4) && PS4) || (defined(PS5) && PS5))
    #define OKAGE_SCREEN_LOAD_NAME     0x008B8B10
    #define OKAGE_SCREEN_LOAD_LOCATION 0x008B98A0
#endif