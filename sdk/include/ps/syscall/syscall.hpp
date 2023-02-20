#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <offsets/ps/libkernel.hpp>

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

namespace PS
{
    #ifdef LIB_KERNEL_SYS_RET
    size_t read(int32_t fd, void* buf, size_t len);
    int32_t readAll(int32_t fd, void* buf, size_t len);
    size_t write(int32_t fd, void* buf, size_t len);
    size_t writeAll(int32_t fd, void* buf, size_t len);
    int32_t open(const char* path, int32_t flags, int32_t mode);
    int32_t close(int32_t fd);
    int32_t unlink(const char* path);
    int32_t rename(const char* from, const char* to);
    int32_t mkdir(const char* path, int32_t mode);
    int32_t rmdir(const char* path);
    int32_t fstat(int32_t fd, struct stat* sb);
    int32_t getdents(uint32_t fd, void* dirp, uint32_t count);
    int32_t kqueue();
    int32_t kevent(int32_t kq, struct kevent *changelist, int32_t nchanges, struct kevent* eventlist, int32_t nevents, struct timespec* timeout);
    int32_t getFunctionAddressByName(int moduleId, char* name, void* destination);
    int32_t getSandboxDirectory(char* buffer, int* length);
    #endif
}
#endif