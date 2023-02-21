#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/breakout.hpp>

namespace PS
{
    namespace Sce
    {
        typedef struct 
        {
            int type;
            int reqId;
            int priority;
            int msgId;
            int targetId;
            int userId;
            int unk1;
            int unk2;
            int appId;
            int errorNum;
            int unk3;
            unsigned char useIconImageUri;
            char message[1024];
            char iconUri[1024];
            char unk[1024];
        } NotificationRequest;

        class Kernel
        {
        public:
            static int32_t Usleep(uint32_t microseconds);
            static int32_t Sleep(uint32_t seconds);
            #ifdef LIBKERNEL
            static int32_t SendNotificationRequest(int32_t device, PS::Sce::NotificationRequest* request, uint64_t size, int32_t blocking);
            static int32_t LoadStartModule(char* name, size_t argc, void* argv, uint32_t flags, int32_t unk1, int32_t unk2);
            static int32_t RandomizedPath(char* buffer, int* length);
            static int32_t Dlsym(int moduleId, char* name, void* destination);
            #endif
        };
    }
}
#endif