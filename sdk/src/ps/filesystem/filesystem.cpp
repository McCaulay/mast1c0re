#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/filesystem/filesystem.hpp>
#include <ps2/ps2.hpp>
#include <ps/ps.hpp>
#include <ps/syscall/syscall.hpp>

int PS::Filesystem::lastIndexOf(const char* str, char c)
{
    return PS2::lastIndexOf(str, c);
}

const char* PS::Filesystem::basename(const char* path)
{
    return PS2::basename(path);
}

char* PS::Filesystem::combine(const char* path, const char* basename)
{
    // Ignore trailing path slash
    int pathLength = PS2::strlen(path);
    if (path[pathLength - 1] == '/')
        pathLength--;

    // Ignore starting basename slash
    int basenameLength = PS2::strlen(basename);
    int basenameOffset = 0;
    if (basename[0] == '/')
        basenameOffset++;

    // Append path and basename
    int newLength = pathLength + basenameLength - basenameOffset + 1/* '/' */;
    char* str = (char*)PS2::malloc(newLength + 1);
    PS2::strncpy(str, path, pathLength);
    str[pathLength] = '/';
    PS2::strncpy(str + pathLength + 1, basename + basenameOffset, newLength);
    str[newLength] = '\0';
    return str;
}

#ifdef LIBKERNEL
bool PS::Filesystem::exists(const char* filepath)
{
    char basename[256];
    PS2::memset(basename, 0, 256);
    char filename[256];
    PS2::memset(filename, 0, 256);

    // Find last "/" in filepath
    int slash = PS::Filesystem::lastIndexOf(filepath, '/');
    if (slash == -1)
    {
        basename[0] = '/';
        basename[1] = '\0';
        PS2::strncpy(filename, filepath, PS2::strlen(filepath));
    }
    else
    {
        PS2::strncpy(basename, filepath, slash);
        basename[slash + 1] = '\0';

        int filenameLen = PS2::strlen(filepath) - slash - 1;
        PS2::strncpy(filename, filepath + slash + 1, filenameLen);
        basename[filenameLen + 1] = '\0';
    }

    int fd = PS::open(basename, 0, 0);
    if (fd <= 0)
        return false;

    bool exitLoop = false;
    for (;;)
    {
        char buffer[8096];
        PS2::memset(buffer, 0, 8096);
        int nread = PS::getdents(fd, buffer, 8096);
        if (nread <= 0)
            break;

        for (int bpos = 0; bpos < nread;)
        {
            struct dirent* dent = (struct dirent*)(buffer + bpos);
            if (dent->d_fileno == 0)
            {
                exitLoop = true;
                break;
            }
            if (PS2::strcmp(dent->d_name, filename) == 0)
            {
                return true;
                PS::close(fd);
            }
            bpos += dent->d_reclen;
        }

        if (exitLoop)
            break;
    }

    PS::close(fd);
    return false;
}

bool PS::Filesystem::deleteIfExists(const char* filepath)
{
    if (!PS::Filesystem::exists(filepath))
        return false;
    PS::unlink(filepath);
    return true;
}

size_t PS::Filesystem::getFileSize(const char* filepath)
{
    int32_t fd = PS::open(filepath, O_RDONLY, 0);
    if (fd <= 0)
        return 0;

    struct stat s;
    PS::fstat(fd, &s);
    PS::close(fd);
    return s.st_size;
}
#endif
#endif