#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/breakout.hpp>

#define MAX_PATH 4096

#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14

#define O_RDONLY  0x0000
#define O_WRONLY  0x0001
#define O_RDWR    0x0002
#define O_ACCMODE 0x0003

#define O_NONBLOCK 0x0004 /* no delay */
#define O_APPEND   0x0008 /* set append mode */
#define O_CREAT    0x0200 /* create if nonexistent */
#define O_TRUNC    0x0400 /* truncate to zero length */
#define O_EXCL     0x0800 /* error if already exists */

#define S_IRWXU 0x00700
#define S_IRUSR 0x00400
#define S_IWUSR 0x00200
#define S_IXUSR 0x00100

#define S_IRWXG 0x00070
#define S_IRGRP 0x00040
#define S_IWGRP 0x00020
#define S_IXGRP 0x00010

#define S_IRWXO 0x00007
#define S_IROTH 0x00004
#define S_IWOTH 0x00002
#define S_IXOTH 0x00001

#define S_ISUID 0x04000
#define S_ISGID 0x02000
#define S_ISVTX 0x01000

#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

struct dirent
{
    uint32_t d_fileno;
    uint16_t d_reclen;
    uint8_t d_type;
    uint8_t d_namlen;
    char d_name[255 + 1];
};

namespace PS
{
    namespace Filesystem
    {
        int lastIndexOf(const char* str, char c);
        const char* basename(const char* path);
        char* combine(const char* path, const char* basename);
        #ifdef LIBKERNEL
        bool exists(const char* filepath);
        bool deleteIfExists(const char* filepath);
        size_t getFileSize(const char* filepath);
        #endif
    };
}
#endif