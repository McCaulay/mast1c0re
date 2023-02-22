#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/syscall/syscall.hpp>
#include <ps/breakout.hpp>

#ifdef LIBKERNEL
#ifndef EBOOT_READ_STUB
size_t PS::read(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__READ), fd, PVAR_TO_NATIVE(buf), len);
}
#endif

#ifndef EBOOT_WRITE_STUB
size_t PS::write(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__WRITE), fd, PVAR_TO_NATIVE(buf), len);
}
#endif

int32_t PS::open(const char* path, int32_t flags, int32_t mode)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__OPEN), PVAR_TO_NATIVE(path), flags, mode);
}

int32_t PS::close(int32_t fd)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__CLOSE), fd);
}

int32_t PS::unlink(const char* path)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_UNLINK), PVAR_TO_NATIVE(path));
}

int32_t PS::getuid()
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_GETUID));
}

#ifndef EBOOT_RENAME_STUB
int32_t PS::rename(const char* from, const char* to)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_RENAME), PVAR_TO_NATIVE(from), PVAR_TO_NATIVE(to));
}
#endif

int32_t PS::mkdir(const char* path, int32_t mode)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_MKDIR), PVAR_TO_NATIVE(path), mode);
}

int32_t PS::rmdir(const char* path)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_RMDIR), PVAR_TO_NATIVE(path));
}

int32_t PS::fstat(int32_t fd, struct stat* sb)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_FSTAT), fd, PVAR_TO_NATIVE(sb));
}

int32_t PS::getdents(uint32_t fd, void* dirp, uint32_t count)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_GETDENTS), fd, PVAR_TO_NATIVE(dirp), count);
}

int32_t PS::kqueue()
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_KQUEUE));
}

int32_t PS::kevent(int32_t kq, struct kevent *changelist, int32_t nchanges, struct kevent* eventlist, int32_t nevents, struct timespec* timeout)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_KEVENT), kq, PVAR_TO_NATIVE(changelist), nchanges, PVAR_TO_NATIVE(eventlist), nevents, PVAR_TO_NATIVE(timeout));
}
#endif

#endif