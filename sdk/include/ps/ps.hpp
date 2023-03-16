#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <cstdint>
#include <stdarg.h>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>
#include <ps/tcp/tcp-client.hpp>

struct stat
{
    dev_t st_dev;            /* inode's device */
    ino_t st_ino;            /* inode's number */
    mode_t st_mode;          /* inode protection mode */
    nlink_t st_nlink;        /* number of hard links */
    uid_t st_uid;            /* user ID of the file's owner */
    gid_t st_gid;            /* group ID of the file's group */
    dev_t st_rdev;           /* device type */
    struct timespec st_atim; /* time of last access */
    struct timespec st_mtim; /* time of last data modification */
    struct timespec st_ctim; /* time of last file status change */
    off_t st_size;           /* file size, in bytes */
    blkcnt_t st_blocks;      /* blocks allocated for file */
    blksize_t st_blksize;    /* optimal blocksize for I/O */
    fflags_t st_flags;       /* user defined flags for file */
    uint32_t st_gen;         /* file generation number */
    int32_t st_lspare;
    struct timespec st_birthtim; /* time of file creation */
    unsigned int : (8 / 2) * (16 - (int)sizeof(struct timespec));
    unsigned int : (8 / 2) * (16 - (int)sizeof(struct timespec));
};

struct kevent
{
    uint64_t ident;  /* identifier for this event */
    int16_t  filter; /* filter for event */
    uint16_t flags;  /* action flags for kqueue */
    uint32_t fflags; /* filter flag value */
    uint64_t data;   /* filter data value */
    void*    udata;  /* opaque user data identifier */
};

#define uxMsg_Snapshot_Save        0x1f4
#define uxMsg_Snapshot_SaveStamped 0x1f5
#define uxMsg_Snapshot_Restore     0x1f6
#define uxMsg_Snapshot_SaveCyclic  0x1f7
#define uxMsg_ResetJIT_EE          0x1fe
#define uxMsg_ResetJIT_IOP         0x1ff
#define uxMsg_ResetJIT_VU          0x200
#define uxMsg_ExitNicely           0x208
#define uxMsg_StopExec             0x212
#define uxMsg_StartExec            0x213
#define uxMsg_ToggleExec           0x214
#define uxMsg_StepExec             0x215
#define uxMsg_EnableToolingMode    0x21c
#define uxMsg_GenCoreDump          0x221
#define uxMsg_GsExternalCommand    0x226
#define uxMsg_RestorePoint_Save    0x22b
#define uxMsg_RestorePoint_Restore 0x22c
#define uxMsg_StartVKLogging       0x230
#define uxMsg_StopVKLogging        0x231
#define uxMsg_SoftReset            0x23a
#define uxMsg_SwitchDisc           0x23b
#define uxMsg_SwitchDiscSwitch     0x23c
#define uxMsg_SwitchDiscClose      0x23d
#define uxMsg_ReloadLuaScripts     0x23e

namespace PS
{
    #ifdef LIBKERNEL
    extern PS::TcpClient Debug;
    #endif

    uint64_t strlen(uint64_t str);
    void exit(int status);
    int feof(uint64_t stream);
    void free(uint64_t ptr);
    int fopen_s(uint64_t pFile, const char* filename, const char* mode);
    int fseek(uint64_t stream, long offset, int whence);
    void malloc_stats();
    uint64_t __error();
    uint64_t memalign(size_t alignment, size_t size);
    uint64_t calloc(size_t num, size_t size);
    void perror(const char* str);
    int strncpy_s(uint64_t dest, size_t destsz, uint64_t src, uint64_t n);
    uint64_t reallocalign(uint64_t ptr, size_t sz, size_t align);
    #ifdef EBOOT_RENAME_STUB
    int rename(const char* from, const char* to);
    #endif
    uint64_t realloc(uint64_t ptr, size_t size);
    int fgetc(uint64_t stream);
    uint64_t malloc(size_t size);
    int stat(const char* pathname, struct stat* statbuf);
    int vsnprintf(uint64_t s, size_t n, uint64_t fmt, va_list arg);
    int vsprintf_s(uint64_t s, size_t n, uint64_t fmt, va_list arg);
    int strerror_s(uint64_t buf, size_t bufsz, int errnum);
    uint64_t memchr(uint64_t ptr, int c, size_t n);
    int memcmp(uint64_t ptr1, uint64_t ptr2, size_t n);
    #ifdef EBOOT_WRITE_STUB
    size_t write(int32_t fd, void* buf, size_t len);
    #endif
    #ifdef EBOOT_READ_STUB
    size_t read(int32_t fd, void* buf, size_t len);
    #endif
    uint64_t memcpy(uint64_t dest, uint64_t src, uint64_t n);
    uint64_t strncpy(uint64_t dest, uint64_t src, uint64_t n);
    uint32_t getpid();

    int32_t PadSetLightBar(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    void SendTrophyPumpCommand(uint32_t command, uint64_t handle, uint64_t trophyId);

    void MountDisc(uint64_t mountDiscOptions);
    char* GetMountedGameCode();
    void SetMountOptionFilepath(const char* filepath);
    void MountDiscWithFilepath(const char* filepath);

    void ProcessConfigFile(const char* filepath);
    // void ReloadLuaScriptsWithDirectory(const char* directory);

    // Emu Commands
    void EmuSendCommand(uint32_t emuMsgId, uint64_t arg = 0);
    void Snapshot_Save(uint64_t arg = 0);
    void Snapshot_SaveStamped(uint64_t arg = 0);
    void Snapshot_Restore(uint64_t arg = 0);
    void Snapshot_SaveCyclic(uint64_t arg = 0);
    void ResetJIT_EE(uint64_t arg = 0);
    void ResetJIT_IOP(uint64_t arg = 0);
    void ResetJIT_VU(uint64_t arg = 0);
    void ExitNicely(uint64_t arg = 0);
    void StopExec(uint64_t arg = 0);
    void StartExec(uint64_t arg = 0);
    void ToggleExec(uint64_t arg = 0);
    void StepExec(uint64_t arg = 0);
    void EnableToolingMode(uint64_t arg = 0);
    void GenCoreDump(uint64_t arg = 0);
    void GsExternalCommand(uint64_t arg = 0);
    void RestorePoint_Save(uint64_t arg = 0);
    void RestorePoint_Restore(uint64_t arg = 0);
    void StartVKLogging(uint64_t arg = 0);
    void StopVKLogging(uint64_t arg = 0);
    void SoftReset(uint64_t arg = 0);
    void SwitchDisc(uint64_t arg = 0);
    void SwitchDiscSwitch(uint64_t arg = 0);
    void SwitchDiscClose(uint64_t arg = 0);
    void ReloadLuaScripts(uint64_t arg = 0);

    int32_t readAll(int32_t fd, void* buf, size_t len);
    size_t writeAll(int32_t fd, void* buf, size_t len);

    #ifdef LIBKERNEL
    void notificationWithIcon(const char* icon, const char* format, ...);
    void notification(const char* format, ...);
    #endif
}
#endif