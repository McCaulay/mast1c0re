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
            static int AllocateDirectMemory(uint64_t searchStart, uint64_t searchEnd, size_t length, size_t align, int type, uint64_t* physicalAddrDest);
            static int MapDirectMemory(uint64_t* virtualAddrDest, size_t length, int protections, int flags, uint64_t physicalAddrDest, size_t align);
            static int GetDirectMemorySize();
            static int CheckReachability(const char* filename);
            static int Ftruncate(int fd, uint64_t length);
            static int Mkdir(const char* path, int mode);
            static size_t Write(int32_t fd, void* buf, size_t len);
            static size_t Pread(int32_t fd, void* buf, size_t len, uint64_t offset);
            static int Open(const char* path, int flags, int mode);
            static int Close(int fd);
            static size_t Read(int32_t fd, void* buf, size_t len);
            static int Stat(const char* pathname, struct stat* statbuf);
            static uint64_t Lseek(int fd, uint64_t offset, int whence);
            static size_t Pwrite(int32_t fd, void* buf, size_t len, uint64_t offset);
            #ifdef LIBKERNEL
            static int32_t SendNotificationRequest(int32_t device, PS::Sce::NotificationRequest* request, uint64_t size, int32_t blocking);
            static int32_t GetIpcPath(char* filepath, char* buffer);
            static int32_t GetCurrentCpu();
            static int32_t GetThreadName(uint32_t id, char* out);
            static int32_t GetCpumode();
            static int32_t LoadStartModule(char* name, size_t argc, void* argv, uint32_t flags, int32_t unk1, int32_t unk2);
            static int32_t RandomizedPath(char* buffer, int* length);
            static int32_t Dlsym(int moduleId, char* name, void* destination);
            #endif
        };
    }
}
#endif