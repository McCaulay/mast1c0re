#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/syscall/syscall.hpp>
#include <ps/syscall/table.hpp>
#include <ps/breakout.hpp>

#if defined(LIBKERNEL) || defined(LIB_KERNEL_SYS_RET_ERROR)
#ifndef EBOOT_READ_STUB
size_t PS::read(int32_t fd, void* buf, size_t len)
{
    #if defined(LIBKERNEL)
    return (size_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__READ), fd, PVAR_TO_NATIVE(buf), len);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (size_t)PS::Breakout::syscall(SYS_READ, fd, PVAR_TO_NATIVE(buf), len);
    #endif
}
#endif

#ifndef EBOOT_WRITE_STUB
size_t PS::write(int32_t fd, void* buf, size_t len)
{
    #if defined(LIBKERNEL)
    return (size_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__WRITE), fd, PVAR_TO_NATIVE(buf), len);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (size_t)PS::Breakout::syscall(SYS_WRITE, fd, PVAR_TO_NATIVE(buf), len);
    #endif
}
#endif

int32_t PS::open(const char* path, int32_t flags, int32_t mode)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__OPEN), PVAR_TO_NATIVE(path), flags, mode);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_OPEN, PVAR_TO_NATIVE(path), flags, mode);
    #endif
}

int32_t PS::close(int32_t fd)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__CLOSE), fd);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_CLOSE, fd);
    #endif
}

int32_t PS::unlink(const char* path)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_UNLINK), PVAR_TO_NATIVE(path));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_UNLINK, PVAR_TO_NATIVE(path));
    #endif
}

int32_t PS::getuid()
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_GETUID));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_GETUID);
    #endif
}

#ifndef EBOOT_RENAME_STUB
int32_t PS::rename(const char* from, const char* to)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_RENAME), PVAR_TO_NATIVE(from), PVAR_TO_NATIVE(to));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_RENAME, PVAR_TO_NATIVE(from), PVAR_TO_NATIVE(to));
    #endif
}
#endif

int32_t PS::mkdir(const char* path, int32_t mode)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_MKDIR), PVAR_TO_NATIVE(path), mode);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_MKDIR, PVAR_TO_NATIVE(path), mode);
    #endif
}

int32_t PS::rmdir(const char* path)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_RMDIR), PVAR_TO_NATIVE(path));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_RMDIR, PVAR_TO_NATIVE(path));
    #endif
}

int32_t PS::fstat(int32_t fd, struct stat* sb)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_FSTAT), fd, PVAR_TO_NATIVE(sb));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_FSTAT2, fd, PVAR_TO_NATIVE(sb));
    #endif
}

#ifdef SYSCALL_SUPPORT
int32_t PS::madvise(uint64_t addr, size_t length, int32_t advice)
{
    return (int32_t)PS::Breakout::syscall(SYS_MADVISE, addr, length, advice);
}
#endif

int32_t PS::getdents(uint32_t fd, void* dirp, uint32_t count)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_GETDENTS), fd, PVAR_TO_NATIVE(dirp), count);
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_GETDENTS, fd, PVAR_TO_NATIVE(dirp), count);
    #endif
}

int32_t PS::kqueue()
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_KQUEUE));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_KQUEUE);
    #endif
}

int32_t PS::kevent(int32_t kq, struct kevent *changelist, int32_t nchanges, struct kevent* eventlist, int32_t nevents, struct timespec* timeout)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_KEVENT), kq, PVAR_TO_NATIVE(changelist), nchanges, PVAR_TO_NATIVE(eventlist), nevents, PVAR_TO_NATIVE(timeout));
    #elif defined(SYSCALL_SUPPORT) && SYSCALL_SUPPORT
    return (int32_t)PS::Breakout::syscall(SYS_KEVENT, kq, PVAR_TO_NATIVE(changelist), nchanges, PVAR_TO_NATIVE(eventlist), nevents, PVAR_TO_NATIVE(timeout));
    #endif
}
#endif

#endif