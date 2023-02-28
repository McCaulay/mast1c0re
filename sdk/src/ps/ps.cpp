#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/ps.hpp>
#include <stdarg.h>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <ps/memory.hpp>
#include <ps/sce/sce.hpp>
#include <ps/sce/libkernel.hpp>

#ifdef LIBKERNEL
PS::TcpClient PS::Debug = PS::TcpClient();
#endif

uint64_t PS::strlen(uint64_t str)
{
    return PS::Breakout::call(EBOOT(EBOOT_STRLEN_STUB), str);
}

void PS::exit(int status)
{
    PS::Breakout::call(EBOOT(EBOOT_EXIT_STUB));
}

int PS::feof(uint64_t stream)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_FEOF_STUB), stream);
}

void PS::free(uint64_t ptr)
{
    PS::Breakout::call(EBOOT(EBOOT_FREE_STUB), ptr);
}

int PS::fopen_s(uint64_t pFile, const char* filename, const char* mode)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_FOPEN_S_STUB), pFile, PVAR_TO_NATIVE(filename), PVAR_TO_NATIVE(mode));
}

int PS::fseek(uint64_t stream, long offset, int whence)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_FSEEK_STUB), stream, offset, whence);
}

void PS::malloc_stats()
{
    PS::Breakout::call(EBOOT(EBOOT_MALLOC_STATS_STUB));
}

uint64_t PS::__error()
{
    return PS::Breakout::call(EBOOT(EBOOT___ERROR_STUB));
}

uint64_t PS::memalign(size_t alignment, size_t size)
{
    return PS::Breakout::call(EBOOT(EBOOT_MEMALIGN_STUB), alignment, size);
}

uint64_t PS::calloc(size_t num, size_t size)
{
    return PS::Breakout::call(EBOOT(EBOOT_CALLOC_STUB), num, size);
}

void PS::perror(const char* str)
{
    PS::Breakout::call(EBOOT(EBOOT_PERROR_STUB), PVAR_TO_NATIVE(str));
}

int PS::strncpy_s(uint64_t dest, size_t destsz, uint64_t src, uint64_t n)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_STRNCPY_S_STUB), dest, destsz, src, n);
}

uint64_t PS::reallocalign(uint64_t ptr, size_t sz, size_t align)
{
    return PS::Breakout::call(EBOOT(EBOOT_REALLOCALIGN_STUB), ptr, sz, align);
}

#ifdef EBOOT_RENAME_STUB
int PS::rename(const char* from, const char* to)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_RENAME_STUB), PVAR_TO_NATIVE(from), PVAR_TO_NATIVE(to));
}
#endif

uint64_t PS::realloc(uint64_t ptr, size_t size)
{
    return PS::Breakout::call(EBOOT(EBOOT_REALLOC_STUB), ptr, size);
}

int PS::fgetc(uint64_t stream)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_FGETC_STUB), stream);
}

uint64_t PS::malloc(size_t size)
{
    return PS::Breakout::call(EBOOT(EBOOT_MALLOC_STUB), size);
}

int PS::stat(const char* pathname, struct stat* statbuf)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_STAT_STUB), PVAR_TO_NATIVE(pathname), PVAR_TO_NATIVE(statbuf));
}

int PS::vsnprintf(uint64_t s, size_t n, uint64_t fmt, va_list arg)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_VSNPRINTF_STUB), s, n, fmt, (uint64_t)arg);
}

int PS::vsprintf_s(uint64_t s, size_t n, uint64_t fmt, va_list arg)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_VSPRINTF_S_STUB), s, n, fmt, (uint64_t)arg);
}

int PS::strerror_s(uint64_t buf, size_t bufsz, int errnum)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_STRERROR_S_STUB), buf, bufsz, errnum);
}

uint64_t PS::memchr(uint64_t ptr, int c, size_t n)
{
    return PS::Breakout::call(EBOOT(EBOOT_MEMCHR_STUB), ptr, c, n);
}

int PS::memcmp(uint64_t ptr1, uint64_t ptr2, size_t n)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_MEMCMP_STUB), ptr1, ptr2, n);
}

#ifdef EBOOT_WRITE_STUB
size_t PS::write(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_WRITE_STUB), fd, PVAR_TO_NATIVE(buf), len);
}
#endif

#ifdef EBOOT_READ_STUB
size_t PS::read(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_READ_STUB), fd, PVAR_TO_NATIVE(buf), len);
}
#endif

uint64_t PS::memcpy(uint64_t dest, uint64_t src, uint64_t n)
{
    return PS::Breakout::call(EBOOT(EBOOT_MEMCPY_STUB), dest, src, n);
}

uint64_t PS::strncpy(uint64_t dest, uint64_t src, uint64_t n)
{
    return PS::Breakout::call(EBOOT(EBOOT_STRNCPY_STUB), dest, src, n);
}

uint32_t PS::getpid()
{
    return (uint32_t)PS::Breakout::call(EBOOT(EBOOT_GETPID_STUB));
}

int32_t PS::PadSetLightBar(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    int32_t colour = alpha;
    colour <<= 8;
    colour |= blue;
    colour <<= 8;
    colour |= green;
    colour <<= 8;
    colour |= red;

    int32_t handle = Memory(EBOOT(EBOOT_PADS)).dereference()->move(4)->read<int32_t>();
    return PS::Sce::PadSetLightBar(handle, &colour);
}

void PS::MountDisc(uint64_t mountDiscOptions)
{
    PS::Breakout::call(EBOOT(EBOOT_MOUNT_DISC_FUNC), mountDiscOptions);
}

void PS::SetMountOptionFilepath(const char* filepath)
{
    uint64_t mountDiscOptions = STACK(EBOOT_MOUNT_DISC_OPTIONS_STACK_OFFSET);
    PS::Memory(mountDiscOptions).move(0x10)->write<uint64_t>(PVAR_TO_NATIVE(filepath));
    PS::Memory(mountDiscOptions).move(0x20)->write<uint64_t>(PS2::strlen(filepath));
}

void PS::MountDiscWithFilepath(const char* filepath)
{
    PS::SetMountOptionFilepath(filepath);
    PS::MountDisc(STACK(EBOOT_MOUNT_DISC_OPTIONS_STACK_OFFSET));
}

char* PS::GetMountedGameCode()
{
    // Get name from mounted file
    return PS::Memory(EBOOT(EBOOT_MOUNT_DISC_GAME_CODE)).move(0x08)->readString();
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

    return total;
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

    return total;
}

#ifdef LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST
void PS::notificationWithIcon(const char* icon, const char* format, ...)
{
    PS::Sce::NotificationRequest request = {
        .type = 0,
        .targetId = -1,
        .unk3 = 0,
        .useIconImageUri = false,
    };

    va_list args;
    va_start(args, format);
    char message[1024];
    PS2::vsprintf(message, format, args);
    va_end(args);

    PS2::strncpy(request.message, message, sizeof(request.message) - 1);
    PS2::strncpy(request.iconUri, icon, sizeof(request.iconUri) - 1);
    PS::Sce::Kernel::SendNotificationRequest(0, &request, sizeof(request), 0);
}

void PS::notification(const char* format, ...)
{
    PS::Sce::NotificationRequest request = {
        .type = 0,
        .targetId = -1,
        .unk3 = 0,
        .useIconImageUri = false,
    };

    va_list args;
    va_start(args, format);
    char message[1024];
    PS2::vsprintf(message, format, args);
    va_end(args);

    PS2::strncpy(request.message, message, sizeof(request.message) - 1);
    PS::Sce::Kernel::SendNotificationRequest(0, &request, sizeof(request), 0);
}
#endif
#endif