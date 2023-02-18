#pragma once

#include <cstdint>

#ifndef NULL
    #define NULL 0x0
#endif

#define MAX_UINT8  0xFF
#define MAX_UINT16 0xFFFF
#define MAX_UINT32 0xFFFFFFFF
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

#define MAX_INT8  0x7F
#define MAX_INT16 0x7FFF
#define MAX_INT32 0x7FFFFFFF
#define MAX_INT64 0x7FFFFFFFFFFFFFFF

typedef uint64_t size_t;
typedef int64_t off_t;

typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef uint32_t pid_t;

typedef int64_t time_t;
typedef long suseconds_t;

typedef uint32_t blksize_t;
typedef int64_t  blkcnt_t;

typedef uint32_t __dev_t;
typedef uint32_t dev_t;
typedef uint32_t fflags_t;
typedef uint32_t ino_t;
typedef uint16_t mode_t;
typedef uint16_t nlink_t;

struct timespec
{
	time_t tv_sec;
	long tv_nsec;
};