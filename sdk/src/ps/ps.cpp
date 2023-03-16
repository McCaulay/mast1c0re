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

void PS::SendTrophyPumpCommand(uint32_t command, uint64_t handle, uint64_t trophyId)
{
    PS::Breakout::call(EBOOT(EBOOT_SEND_TROPHY_PUMP_COMMAND_FUNC), command, handle, trophyId);
}

void PS::MountDisc(uint64_t mountDiscOptions)
{
    PS::Breakout::call(EBOOT(EBOOT_MOUNT_DISC_FUNC), mountDiscOptions);
}

char* PS::GetMountedGameCode()
{
    return PS::Memory(EBOOT(EBOOT_MOUNT_DISC_GAME_CODE)).move(0x08)->readString();
}

void PS::SetMountOptionFilepath(const char* filepath)
{
    uint64_t mountDiscOptions = STACK(EBOOT_MOUNT_DISC_OPTIONS_STACK_OFFSET);
    PS::Memory(mountDiscOptions).move(0x08)->writeStdString(filepath);
}

void PS::MountDiscWithFilepath(const char* filepath)
{
    PS::SetMountOptionFilepath(filepath);
    PS::MountDisc(STACK(EBOOT_MOUNT_DISC_OPTIONS_STACK_OFFSET));
}

void PS::ProcessConfigFile(const char* filepath)
{
    PS::Breakout::call(EBOOT(EBOOT_PROCESS_CONFIG_FILE_FUNC), PVAR_TO_NATIVE(filepath));
}

// void PS::ReloadLuaScriptsWithDirectory(const char* directory)
// {
//     // LuaConfigDirectory = /app0/patches (becomes /app0/patches/SCUS-97129_config.lua)
//     uint64_t LuaConfigDirectory = EBOOT(EBOOT_LUA_CONFIG_DIRECTORY);
//     PS::Memory(LuaConfigDirectory).writeStdString(directory);

//     // LuaTrophyDirectory = /app0/trophy_data (becomes /app0/trophy_data/SCUS-97129_trophies.lua)
//     uint64_t LuaTrophyDirectory = EBOOT(EBOOT_LUA_TROPHY_DIRECTORY);
//     // PS::Memory(LuaTrophyDirectory).writeStdString(directory);

//     // LuaFeatureDirectory = /app0/feature_data (becomes /app0/feature_data/SCUS-97129_features.lua)
//     uint64_t LuaFeatureDirectory = EBOOT(EBOOT_LUA_FEATURE_DIRECTORY);
//     // PS::Memory(LuaFeatureDirectory).writeStdString(directory);

//     // LuaToolingDirectory = ./tooling/%(TITLE_ID) (becomes ./tooling/SCUS-97129/SCUS-97129_tooling.lua)
//     uint64_t LuaToolingDirectory = EBOOT(EBOOT_LUA_TOOLING_DIRECTORY);
//     // PS::Memory(LuaToolingDirectory).writeStdString(directory);

//     // LuaConfigLocalFile = ./config-local.lua
//     // uint64_t LuaConfigLocalFile = EBOOT(EBOOT_LUA_LOCAL_CONFIG_FILE);

//     PS::ReloadLuaScripts();
// }

void PS::EmuSendCommand(uint32_t emuMsgId, uint64_t arg)
{
    // Setup command
    uint64_t ptr = PS::Memory(EBOOT(EBOOT_EMU_COMMAND)).dereference()->dereference()->getAddress();

    PS::Memory::write<uint32_t>(ptr + 0x10, emuMsgId);
    PS::Memory::write<uint64_t>(ptr + 0x18, arg);

    // Trigger command
    PS::Memory(EBOOT(EBOOT_EMU_COMMAND_COUNTER)).write<uint64_t>(1);
}

void PS::Snapshot_Save(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_Snapshot_Save, arg);
}

void PS::Snapshot_SaveStamped(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_Snapshot_SaveStamped, arg);
}

void PS::Snapshot_Restore(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_Snapshot_Restore, arg);
}

void PS::Snapshot_SaveCyclic(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_Snapshot_SaveCyclic, arg);
}

void PS::ResetJIT_EE(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ResetJIT_EE, arg);
}

void PS::ResetJIT_IOP(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ResetJIT_IOP, arg);
}

void PS::ResetJIT_VU(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ResetJIT_VU, arg);
}

void PS::ExitNicely(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ExitNicely, arg);
}

void PS::StopExec(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_StopExec, arg);
}

void PS::StartExec(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_StartExec, arg);
}

void PS::ToggleExec(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ToggleExec, arg);
}

void PS::StepExec(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_StepExec, arg);
}

void PS::EnableToolingMode(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_EnableToolingMode, arg);
}

void PS::GenCoreDump(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_GenCoreDump, arg);
}

void PS::GsExternalCommand(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_GsExternalCommand, arg);
}

void PS::RestorePoint_Save(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_RestorePoint_Save, arg);
}

void PS::RestorePoint_Restore(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_RestorePoint_Restore, arg);
}

void PS::StartVKLogging(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_StartVKLogging, arg);
}

void PS::StopVKLogging(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_StopVKLogging, arg);
}

void PS::SoftReset(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_SoftReset, arg);
}

void PS::SwitchDisc(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_SwitchDisc, arg);
}

void PS::SwitchDiscSwitch(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_SwitchDiscSwitch, arg);
}

void PS::SwitchDiscClose(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_SwitchDiscClose, arg);
}

void PS::ReloadLuaScripts(uint64_t arg)
{
    PS::EmuSendCommand(uxMsg_ReloadLuaScripts, arg);
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