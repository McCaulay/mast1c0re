#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/libkernel.hpp>
#include <ps/syscall/table.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>

int32_t PS::Sce::Kernel::Usleep(uint32_t microseconds)
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_USLEEP_STUB), (uint64_t)microseconds);
}

int32_t PS::Sce::Kernel::Sleep(uint32_t seconds)
{
    return PS::Sce::Kernel::Usleep(seconds * 1000000);
}

int PS::Sce::Kernel::AllocateDirectMemory(uint64_t searchStart, uint64_t searchEnd, size_t length, size_t align, int type, uint64_t* physicalAddrDest)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_ALLOCATE_DIRECT_MEMORY_STUB), searchStart, searchEnd, length, align, type, PVAR_TO_NATIVE(physicalAddrDest));
}

int PS::Sce::Kernel::MapDirectMemory(uint64_t* virtualAddrDest, size_t length, int protections, int flags, uint64_t physicalAddrDest, size_t align)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_MAP_DIRECT_MEMORY_STUB), PVAR_TO_NATIVE(virtualAddrDest), length, protections, flags, physicalAddrDest, align);
}

int PS::Sce::Kernel::GetDirectMemorySize()
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_GET_DIRECT_MEMORY_SIZE_STUB));
}

int PS::Sce::Kernel::CheckReachability(const char* filename)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_CHECK_REACHABILITY_STUB), PVAR_TO_NATIVE(filename));
}

int PS::Sce::Kernel::Ftruncate(int fd, uint64_t length)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_FTRUNCATE_STUB), fd, length);
}

int PS::Sce::Kernel::Mkdir(const char* path, int mode)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_MKDIR_STUB), PVAR_TO_NATIVE(path), mode);
}

size_t PS::Sce::Kernel::Write(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_WRITE_STUB), fd, PVAR_TO_NATIVE(buf), len);
}

size_t PS::Sce::Kernel::Pread(int32_t fd, void* buf, size_t len, uint64_t offset)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_PREAD_STUB), fd, PVAR_TO_NATIVE(buf), len, offset);
}

int PS::Sce::Kernel::Open(const char* path, int flags, int mode)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_OPEN_STUB), PVAR_TO_NATIVE(path), flags, mode);
}

int PS::Sce::Kernel::Close(int fd)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_CLOSE_STUB), fd);
}

size_t PS::Sce::Kernel::Read(int32_t fd, void* buf, size_t len)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_READ_STUB), fd, PVAR_TO_NATIVE(buf), len);
}

int PS::Sce::Kernel::Stat(const char* pathname, struct stat* statbuf)
{
    return (int)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_STAT_STUB), PVAR_TO_NATIVE(pathname), PVAR_TO_NATIVE(statbuf));
}

uint64_t PS::Sce::Kernel::Lseek(int fd, uint64_t offset, int whence)
{
    return PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_LSEEK_STUB), fd, offset, whence);
}

size_t PS::Sce::Kernel::Pwrite(int32_t fd, void* buf, size_t len, uint64_t offset)
{
    return (size_t)PS::Breakout::call(EBOOT(EBOOT_SCE_KERNEL_PWRITE_STUB), fd, PVAR_TO_NATIVE(buf), len, offset);
}

#ifdef LIBKERNEL
int32_t PS::Sce::Kernel::SendNotificationRequest(int32_t device, PS::Sce::NotificationRequest* request, uint64_t size, int32_t blocking)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST), device, PVAR_TO_NATIVE(request), size, blocking);
}

int32_t PS::Sce::Kernel::GetIpcPath(char* filepath, char* buffer)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_GET_IPC_PATH), PVAR_TO_NATIVE(filepath), PVAR_TO_NATIVE(buffer));
}

int32_t PS::Sce::Kernel::GetCurrentCpu()
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_GET_CURRENT_CPU));
}

int32_t PS::Sce::Kernel::GetThreadName(uint32_t id, char* out)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_GET_THREAD_NAME), id, PVAR_TO_NATIVE(out));
}

int32_t PS::Sce::Kernel::GetCpumode()
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_GET_CPUMODE));
}

int32_t PS::Sce::Kernel::LoadStartModule(const char* name, size_t argc, void* argv, uint32_t flags, int32_t unk1, int32_t unk2)
{
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_LOAD_START_MODULE), PVAR_TO_NATIVE(name), argc, PVAR_TO_NATIVE(argv), flags, unk1, unk2);
}
#endif

#if defined(LIBKERNEL) || defined(LIB_KERNEL_SYS_RET_ERROR)
int32_t PS::Sce::Kernel::RandomizedPath(char* buffer, int* length)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_RANDOMIZED_PATH), 0, PVAR_TO_NATIVE(buffer), PVAR_TO_NATIVE(length));
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_RANDOMIZED_PATH, 0, PVAR_TO_NATIVE(buffer), PVAR_TO_NATIVE(length));
    #endif
}

int32_t PS::Sce::Kernel::Dlsym(int moduleId, const char* name, void* destination)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SCE_KERNEL_DLSYM), moduleId, PVAR_TO_NATIVE(name), PVAR_TO_NATIVE(destination));
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_DYNLIB_DLSYM, moduleId, PVAR_TO_NATIVE(name), PVAR_TO_NATIVE(destination));
    #endif
}
#endif
#endif