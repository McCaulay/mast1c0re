#pragma once
#include <types.hpp>

class Okage
{
public:
    typedef int fMemoryCardOpen(const char* name, int mode);
    typedef void fMemoryCardRead(int fd, uint8_t* buffer, int size);
    typedef void fMemoryCardWrite(int fd, uint8_t* buffer, int size);
    typedef void fMemoryCardSeek(int fd, int offset, int origin);
    typedef void fMemoryCardClose(int fd);
public:
    static fMemoryCardOpen*  MemoryCardOpen;
    static fMemoryCardRead*  MemoryCardRead;
    static fMemoryCardWrite* MemoryCardWrite;
    static fMemoryCardSeek*  MemoryCardSeek;
    static fMemoryCardClose* MemoryCardClose;
public:
    static void clearScreenText();
    static int printf(const char* message, ...);
};