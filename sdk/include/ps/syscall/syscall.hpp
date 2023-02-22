#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>
#include <ps/ps.hpp>

namespace PS
{
    #ifdef LIBKERNEL
    #ifndef EBOOT_READ_STUB
    size_t read(int32_t fd, void* buf, size_t len);
    #endif
    #ifndef EBOOT_WRITE_STUB
    size_t write(int32_t fd, void* buf, size_t len);
    #endif
    int32_t open(const char* path, int32_t flags, int32_t mode);
    int32_t close(int32_t fd);
    int32_t unlink(const char* path);
    int32_t getuid();
    #ifndef EBOOT_RENAME_STUB
    int32_t rename(const char* from, const char* to);
    #endif
    int32_t mkdir(const char* path, int32_t mode);
    int32_t rmdir(const char* path);
    int32_t fstat(int32_t fd, struct stat* sb);
    int32_t getdents(uint32_t fd, void* dirp, uint32_t count);
    int32_t kqueue();
    int32_t kevent(int32_t kq, struct kevent *changelist, int32_t nchanges, struct kevent* eventlist, int32_t nevents, struct timespec* timeout);
    #endif
}
#endif