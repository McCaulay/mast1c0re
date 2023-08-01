#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>
#include <ps/ps.hpp>

/*
 * Advice to madvise
 */
#define MADV_NORMAL     0       /* no further special treatment */
#define MADV_RANDOM     1       /* expect random page references */
#define MADV_SEQUENTIAL 2       /* expect sequential page references */
#define MADV_WILLNEED   3       /* will need these pages */
#define MADV_DONTNEED   4       /* dont need these pages */
#define MADV_FREE       5       /* dont need these pages, and junk contents */
#define MADV_NOSYNC     6       /* try to avoid flushes to physical media */
#define MADV_AUTOSYNC   7       /* revert to default flushing strategy */
#define MADV_NOCORE     8       /* do not include these pages in a core file */
#define MADV_CORE       9       /* revert to including pages in a core file */
#define MADV_PROTECT    10      /* protect process from pageout kill */

namespace PS
{
    #if defined(LIBKERNEL) || defined(LIB_KERNEL_SYS_RET_ERROR)
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
    #ifdef SYSCALL_SUPPORT
    int32_t madvise(uint64_t addr, size_t length, int32_t advice);
    #endif
    int32_t getdents(uint32_t fd, void* dirp, uint32_t count);
    int32_t kqueue();
    int32_t kevent(int32_t kq, struct kevent *changelist, int32_t nchanges, struct kevent* eventlist, int32_t nevents, struct timespec* timeout);
    #endif
}
#endif