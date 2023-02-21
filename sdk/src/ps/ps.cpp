#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/ps.hpp>
#include <stdarg.h>
#include <offsets/ps/eboot/eboot.hpp>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <ps/memory.hpp>
#include <ps/sce/sce.hpp>
#include <ps/sce/libkernel.hpp>

#ifdef LIBKERNEL
PS::TcpClient PS::Debug = PS::TcpClient();
#endif

uint64_t PS::memcpy(uint64_t dest, uint64_t src, uint64_t n)
{
    return PS::Breakout::call(EBOOT(EBOOT_SCE_MEMCPY_STUB), dest, src, n);
}

uint64_t PS::strncpy(uint64_t dest, uint64_t src, uint64_t n)
{
    return PS::Breakout::call(EBOOT(EBOOT_SCE_STRNCPY_STUB), dest, src, n);
}

uint32_t PS::getpid()
{
    return (uint32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_GETPID_STUB));
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