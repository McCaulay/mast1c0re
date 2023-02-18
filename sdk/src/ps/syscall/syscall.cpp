#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/syscall/syscall.hpp>
#include <ps/breakout.hpp>

#ifdef LIB_KERNEL_LEAKED
size_t PS::read(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::syscall(3, fd, PVAR_TO_NATIVE(buf), len);
}

int32_t PS::readAll(int32_t fd, void* buf, size_t len)
{
    size_t total = 0;
    int32_t count = 0;

    while ((count = PS::read(fd, (uint8_t*)buf + total, len - total)) > 0)
    {
        total += count;
        if (total >= len)
            break;
    }

    if (total >= len)
        return total;

    if (count == -1)
        return -1;

    return total;
}

size_t PS::write(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::syscall(4, fd, PVAR_TO_NATIVE(buf), len);
}

size_t PS::writeAll(int32_t fd, void* buf, size_t len)
{
    size_t total = 0;
    size_t count = 0;

    while ((count = PS::write(fd, (uint8_t*)buf + total, len - total)) > 0)
    {
        total += count;
        if (total >= len)
            break;
    }

    if (total >= len)
        return total;

    if (count == -1)
        return -1;

    return total;
}

int32_t PS::open(const char* path, int32_t flags, int32_t mode)
{
    return (int32_t)PS::Breakout::syscall(5, PVAR_TO_NATIVE(path), flags, mode);
}

int32_t PS::close(int32_t fd)
{
    return (int32_t)PS::Breakout::syscall(6, fd);
}

int32_t PS::unlink(const char* path)
{
    return (int32_t)PS::Breakout::syscall(10, PVAR_TO_NATIVE(path));
}

int32_t PS::rename(const char* from, const char* to)
{
    return (int32_t)PS::Breakout::syscall(128, PVAR_TO_NATIVE(from), PVAR_TO_NATIVE(to));
}

int32_t PS::mkdir(const char* path, int32_t mode)
{
    return (int32_t)PS::Breakout::syscall(136, PVAR_TO_NATIVE(path), mode);
}

int32_t PS::rmdir(const char* path)
{
    return (int32_t)PS::Breakout::syscall(137, PVAR_TO_NATIVE(path));
}

int32_t PS::fstat(int32_t fd, struct stat* sb)
{
    return (int32_t)PS::Breakout::syscall(189, fd, PVAR_TO_NATIVE(sb));
}

int32_t PS::getdents(uint32_t fd, void* dirp, uint32_t count)
{
    return (int32_t)PS::Breakout::syscall(272, fd, PVAR_TO_NATIVE(dirp), count);
}

int32_t PS::getFunctionAddressByName(int moduleId, char* name, void* destination)
{
    return (int32_t)PS::Breakout::syscall(591, moduleId, PVAR_TO_NATIVE(name), PVAR_TO_NATIVE(destination));
}

int32_t PS::getSandboxDirectory(char* buffer, int* length)
{
    return (int32_t)PS::Breakout::syscall(602, 0, PVAR_TO_NATIVE(buffer), PVAR_TO_NATIVE(length));
}
#endif
#endif