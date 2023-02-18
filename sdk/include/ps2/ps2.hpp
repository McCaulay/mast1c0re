#pragma once
#include <stdarg.h>

#define PS2_MAX_THREADS 255

class PS2
{
public:
    // common
    typedef void* fMalloc(unsigned int size);
    typedef void  fFree(void* addr);
    typedef char* fStrncpy(char* destination, const char* source, unsigned int num);
    typedef int   fStrcmp(const char* str1, const char* str2);
    typedef int   fStrlen(const char* str);
    typedef void* fMemcpy(void* dest, void* src, unsigned int n);
    typedef int   fSprintf(char* str, const char* format, ...);
    typedef int   fVsprintf(char* str, const char* format, va_list arg);

    // syscalls
    typedef void fLoadExecPS2(char* filename, int argc, char** argv);
    typedef int  fGetThreadId();
    typedef int  fSleepThread();
    typedef int  fSuspendThread(int threadId);
    typedef int  fTerminateThread(int threadId);
    typedef int  fDeleteThread(int threadId);
    typedef void fExit(int code);

    // libmc
    typedef int fMcInit(int type);
    typedef int fMcGetInfo(int port, int slot, int* type, int* free, int* format);
    typedef int fMcOpen(int port, int slot, char *name, int mode);
    typedef int fMcClose(int fd);
    typedef int fMcSeek(int fd, int offset, int origin);
    typedef int fMcRead(int fd, void *buffer, int size);
    typedef int fMcWrite(int fd, void *buffer, int size);
    typedef int fMcFlush(int fd);
    typedef int fMcMkDir(int port, int slot, char* name);
    typedef int fMcChdir(int port, int slot, char* newDir, char* currentDir);
    typedef int fMcGetDir(int port, int slot, char *name, unsigned mode, int maxent, unsigned char* table);
    typedef int fMcSetFileInfo(int port, int slot, char* name, unsigned char* info, unsigned flags);
    typedef int fMcDelete(int port, int slot, char *name);
    typedef int fMcFormat(int port, int slot);
    typedef int fMcUnformat(int port, int slot);
    typedef int fMcGetEntSpace(int port, int slot, char* path);
    typedef int fMcRename(int port, int slot, char* oldName, char* newName);
    typedef int fMcChangeThreadPriority(int level);
    typedef int fMcSync(int mode, int *cmd, int *result);
public:
    // common
    static fMalloc*   malloc;
    static fFree*     free;
    static fStrncpy*  strncpy;
    static fStrcmp*   strcmp;
    static fStrlen*   strlen;
    static fMemcpy*   memcpy;
    static fSprintf*  sprintf;
    static fVsprintf* vsprintf;

    // syscalls
    static fLoadExecPS2*     LoadExecPS2;
    static fGetThreadId*     GetThreadId;
    static fSleepThread*     SleepThread;
    static fSuspendThread*   SuspendThread;
    static fTerminateThread* TerminateThread;
    static fDeleteThread*    DeleteThread;
    static fExit*            Exit;

    // libmc
    static fMcInit*                 mcInit;
    static fMcGetInfo*              mcGetInfo;
    static fMcOpen*                 mcOpen;
    static fMcClose*                mcClose;
    static fMcSeek*                 mcSeek;
    static fMcRead*                 mcRead;
    static fMcWrite*                mcWrite;
    static fMcFlush*                mcFlush;
    static fMcMkDir*                mcMkDir;
    static fMcChdir*                mcChdir;
    static fMcGetDir*               mcGetDir;
    static fMcSetFileInfo*          mcSetFileInfo;
    static fMcDelete*               mcDelete;
    static fMcFormat*               mcFormat;
    static fMcUnformat*             mcUnformat;
    static fMcGetEntSpace*          mcGetEntSpace;
    static fMcRename*               mcRename;
    static fMcChangeThreadPriority* mcChangeThreadPriority;
    static fMcSync*                 mcSync;
public:
    static void* memset(void* str, int c, int n);
    static void* realloc(void* ptr, unsigned int size);
    static char* strcpy(char* dest, const char* src);
    static char* strcat(char* dest, const char* src);
    static void killThreads();
    static char* gameCodeToPath(char* gameCode);
};