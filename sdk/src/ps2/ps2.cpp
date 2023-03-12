#include <types.hpp>
#include <ps2/ps2.hpp>
#include <compiler.hpp>
#include <offsets/okage.hpp>
#include <cstdint>

// common
PS2::fMalloc*   PS2::malloc   = (PS2::fMalloc*)PS2_MALLOC;
PS2::fFree*     PS2::free     = (PS2::fFree*)PS2_FREE;
PS2::fStrncpy*  PS2::strncpy  = (PS2::fStrncpy*)PS2_STRNCPY;
PS2::fStrcmp*   PS2::strcmp   = (PS2::fStrcmp*)PS2_STRCMP;
PS2::fStrlen*   PS2::strlen   = (PS2::fStrlen*)PS2_STRLEN;
PS2::fMemcpy*   PS2::memcpy   = (PS2::fMemcpy*)PS2_MEMCPY;
PS2::fSprintf*  PS2::sprintf  = (PS2::fSprintf*)PS2_SPRINTF;
PS2::fVsprintf* PS2::vsprintf = (PS2::fVsprintf*)PS2_VSPRINTF;

// syscalls
PS2::fResetEE*          PS2::ResetEE          = (PS2::fResetEE*)PS2_SYS_RESET_EE;
PS2::fLoadExecPS2*      PS2::LoadExecPS2      = (PS2::fLoadExecPS2*)PS2_SYS_LOAD_EXEC_PS2;
PS2::fExecPS2*          PS2::ExecPS2          = (PS2::fExecPS2*)PS2_SYS_EXEC_PS2;
PS2::fGetThreadId*      PS2::GetThreadId      = (PS2::fGetThreadId*)PS2_SYS_GET_THREAD_ID;
PS2::fCreateThread*     PS2::CreateThread     = (PS2::fCreateThread*)PS2_SYS_CREATE_THREAD;
PS2::fStartThread*      PS2::StartThread      = (PS2::fStartThread*)PS2_SYS_START_THREAD;
PS2::fSleepThread*      PS2::SleepThread      = (PS2::fSleepThread*)PS2_SYS_SLEEP_THREAD;
PS2::fExitDeleteThread* PS2::ExitDeleteThread = (PS2::fExitDeleteThread*)PS2_SYS_EXIT_DELETE_THREAD;;
PS2::fSuspendThread*    PS2::SuspendThread    = (PS2::fSuspendThread*)PS2_SYS_SUSPEND_THREAD;
PS2::fTerminateThread*  PS2::TerminateThread  = (PS2::fTerminateThread*)PS2_SYS_TERMINATE_THREAD;
PS2::fDeleteThread*     PS2::DeleteThread     = (PS2::fDeleteThread*)PS2_SYS_DELETE_THREAD;
PS2::fExit*             PS2::Exit             = (PS2::fExit*)PS2_SYS_EXIT;

// Sif
PS2::fSifIopReset*      PS2::SifIopReset = (PS2::fSifIopReset*)PS2_SIF_IOP_RESET;
PS2::fSifIopSync*       PS2::SifIopSync  = (PS2::fSifIopSync*)PS2_SIF_IOP_SYNC;
PS2::fSifInitRpc*       PS2::SifInitRpc  = (PS2::fSifInitRpc*)PS2_SIF_INIT_RPC;
PS2::fSifExitRpc*       PS2::SifExitRpc  = (PS2::fSifExitRpc*)PS2_SIF_EXIT_RPC;

// libmc
PS2::fMcInit*                 PS2::mcInit                 = (PS2::fMcInit*)PS2_MC_INIT;
PS2::fMcGetInfo*              PS2::mcGetInfo              = (PS2::fMcGetInfo*)PS2_MC_GET_INFO;
PS2::fMcOpen*                 PS2::mcOpen                 = (PS2::fMcOpen*)PS2_MC_OPEN;
PS2::fMcClose*                PS2::mcClose                = (PS2::fMcClose*)PS2_MC_CLOSE;
PS2::fMcSeek*                 PS2::mcSeek                 = (PS2::fMcSeek*)PS2_MC_SEEK;
PS2::fMcRead*                 PS2::mcRead                 = (PS2::fMcRead*)PS2_MC_READ;
PS2::fMcWrite*                PS2::mcWrite                = (PS2::fMcWrite*)PS2_MC_WRITE;
PS2::fMcFlush*                PS2::mcFlush                = (PS2::fMcFlush*)PS2_MC_FLUSH;
PS2::fMcMkDir*                PS2::mcMkDir                = (PS2::fMcMkDir*)PS2_MC_MK_DIR;
PS2::fMcChdir*                PS2::mcChdir                = (PS2::fMcChdir*)PS2_MC_CH_DIR;
PS2::fMcGetDir*               PS2::mcGetDir               = (PS2::fMcGetDir*)PS2_MC_GET_DIR;
PS2::fMcSetFileInfo*          PS2::mcSetFileInfo          = (PS2::fMcSetFileInfo*)PS2_MC_SET_FILE_INFO;
PS2::fMcDelete*               PS2::mcDelete               = (PS2::fMcDelete*)PS2_MC_DELETE;
PS2::fMcFormat*               PS2::mcFormat               = (PS2::fMcFormat*)PS2_MC_FORMAT;
PS2::fMcUnformat*             PS2::mcUnformat             = (PS2::fMcUnformat*)PS2_MC_UNFORMAT;
PS2::fMcGetEntSpace*          PS2::mcGetEntSpace          = (PS2::fMcGetEntSpace*)PS2_MC_GET_ENT_SPACE;
PS2::fMcRename*               PS2::mcRename               = (PS2::fMcRename*)PS2_MC_RENAME;
PS2::fMcChangeThreadPriority* PS2::mcChangeThreadPriority = (PS2::fMcChangeThreadPriority*)PS2_MC_CHANGE_THREAD_PRIORITY;
PS2::fMcSync*                 PS2::mcSync                 = (PS2::fMcSync*)PS2_MC_SYNC;

int PS2::memcmp(void* p1, void* p2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (*(uint8_t*)p1 != *(uint8_t*)p2)
            return -1;
    }
    return 0;
}

void* PS2::memset(void* str, int c, int n)
{
    return ::memset(str, c, n);
}

void* PS2::realloc(void* ptr, unsigned int size)
{
    void* newPtr = PS2::malloc(size);
    PS2::memcpy(newPtr, ptr, size);
    PS2::free(ptr);
    return newPtr;
}

char* PS2::strcpy(char* dest, const char* src)
{
    int srcLen = PS2::strlen(src);
    for (int i = 0; i < srcLen; i++)
        dest[i] = src[i];
    return dest;
}

char* PS2::strcat(char* dest, const char* src)
{
    int destLen = PS2::strlen(dest);
    int srcLen = PS2::strlen(src);
    PS2::strcpy(dest + destLen, src);
    dest[destLen + srcLen] = '\0';
    return dest;
}

bool PS2::contains(const char* str, const char* c)
{
    return PS2::indexOf(str, c) != -1;
}

int PS2::indexOf(const char* str, const char* c, int fromOffset)
{
    int len = PS2::strlen(str);
    int cLen = PS2::strlen(c);
    for (int i = fromOffset; i < len; i++)
    {
        if (PS2::memcmp((void*)(str + i), (void*)c, cLen) == 0)
            return i;
    }
    return -1;
}

int PS2::indexOf(const char* str, char c, int fromOffset)
{
    int len = PS2::strlen(str);
    for (int i = fromOffset; i < len; i++)
    {
        if (str[i] == c)
            return i;
    }
    return -1;
}

int PS2::lastIndexOf(const char* str, char c)
{
    int len = PS2::strlen(str);
    for (int i = len - 1; i >= 0; i--)
    {
        if (str[i] == c)
            return i;
    }
    return -1;
}

const char* PS2::basename(const char* path)
{
    // "usr" -> "usr"
    int slash = PS2::lastIndexOf(path, '/');
    if (slash == -1)
        return path;

    // "/" -> "/"
    if (PS2::strlen(path) == 1 && path[0] == '/')
        return path;

    // "/usr" -> "usr"
    // "/EBOOT.BIN" -> "EBOOT.BIN"
    return path + slash + 1;
}

int PS2::createAndStartThread(void *(*func)(void*), void* stack, uint32_t stackSize, const char* name)
{
    ee_thread_t thread;
    thread.func = func;
    thread.stack = stack;
    thread.stack_size = stackSize;
    thread.option = (uint32_t)name;
    thread.initial_priority = 0;
    thread.gp_reg = 0;
    int threadId = PS2::CreateThread(&thread);
    if (threadId < 0)
        return threadId;

    int result = PS2::StartThread(threadId, nullptr);
    if (result < 0)
        return result;
    return threadId;
}

void PS2::killThreads()
{
    // Get current thread id
    int tid = PS2::GetThreadId();

    // Iterate each thread
    for (int i = 1; i < PS2_MAX_THREADS; i++)
    {
        // Terminate and delete thread if it is not the current thread
        if (i != tid)
        {
            PS2::TerminateThread(i);
            PS2::DeleteThread(i);
        }
    }
}

char* PS2::gameCodeToPath(char* gameCode)
{
    char* path = (char*)PS2::malloc(22);
    PS2::strncpy(path, "cdrom0:\\", 8);
    PS2::strncpy(path + 8, gameCode, 4);
    PS2::strncpy(path + 12, "_", 1);
    PS2::strncpy(path + 13, gameCode + 5, 3);
    PS2::strncpy(path + 16, ".", 1);
    PS2::strncpy(path + 17, gameCode + 8, 2);
    PS2::strncpy(path + 19, ";1", 2);
    path[21] = '\0';
    return path;
}