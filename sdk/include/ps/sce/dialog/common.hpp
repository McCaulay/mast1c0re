#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>

#define SCE_COMMON_DIALOG_MAGIC_NUMBER 0xC0D1A109

#define SCE_COMMON_DIALOG_ERROR_ALREADY_CLOSE              0x80b8000c // CE-33413-4
#define SCE_COMMON_DIALOG_ERROR_ALREADY_INITIALIZED        0x80b80004 // CE-33405-5
#define SCE_COMMON_DIALOG_ERROR_ALREADY_SYSTEM_INITIALIZED 0x80b80002 // CE-33403-3
#define SCE_COMMON_DIALOG_ERROR_ARG_NULL                   0x80b8000d // CE-33414-5
#define SCE_COMMON_DIALOG_ERROR_BUSY                       0x80b80008 // CE-33409-9
#define SCE_COMMON_DIALOG_ERROR_INVALID_STATE              0x80b80006 // CE-33407-7
#define SCE_COMMON_DIALOG_ERROR_NOT_FINISHED               0x80b80005 // CE-33406-6
#define SCE_COMMON_DIALOG_ERROR_NOT_INITIALIZED            0x80b80003 // CE-33404-4
#define SCE_COMMON_DIALOG_ERROR_NOT_RUNNING                0x80b8000b // CE-33412-3
#define SCE_COMMON_DIALOG_ERROR_NOT_SUPPORTED              0x80b8000f // CE-33416-7
#define SCE_COMMON_DIALOG_ERROR_NOT_SYSTEM_INITIALIZED     0x80b80001 // CE-33402-2
#define SCE_COMMON_DIALOG_ERROR_OUT_OF_MEMORY              0x80b80009 // CE-33410-1
#define SCE_COMMON_DIALOG_ERROR_PARAM_INVALID              0x80b8000a // CE-33411-2
#define SCE_COMMON_DIALOG_ERROR_RESULT_NONE                0x80b80007 // CE-33408-8
#define SCE_COMMON_DIALOG_ERROR_UNEXPECTED_FATAL           0x80b8000e // CE-33415-6
#define SCE_COMMON_DIALOG_SERVER_ERROR_ALREADY_FINISHED    0x80c60001 // CE-33597-7
#define SCE_COMMON_DIALOG_SERVER_ERROR_NOT_SETDATA         0x80c60000 // CE-33596-6

namespace PS
{
    namespace Sce
    {
        class CommonDialog
        {
        public:
            typedef enum
            {
                OK = 0,
                USER_CANCELED = 1
            } Result;

            typedef struct
            {
                size_t size;
                uint8_t reserved[36];
                uint32_t magic;
            } BaseParam __attribute__ ((__aligned__(8)));

            typedef enum
            {
                NONE = 0,
                INITIALIZED = 1,
                RUNNING = 2,
                FINISHED = 3
            } Status;
        public:
            static int32_t Initialize();
            static bool IsUsed();
        private:
            static bool isInitialized;
        public:
            CommonDialog();
        };
    }
}
#endif