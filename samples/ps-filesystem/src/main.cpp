#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Attempt to connect to debug server
    PS::Debug.connect(IP(192, 168, 0, 7), 9023);

    // Print directories/files
    const char* directory = "/";
    int fd = PS::open(directory, 0, 0);
    if (fd > 0)
    {
        char buffer[1024];
        int nread = 0;
        while ((nread = PS::getdents(fd, buffer, sizeof(buffer))) > 0)
        {
            struct dirent* dent = (struct dirent*)buffer;
            while (dent->d_fileno)
            {
                if (dent->d_type == DT_DIR)
                    PS::Debug.printf("d %s%s\n", directory, dent->d_name);
                if (dent->d_type == DT_REG)
                    PS::Debug.printf("- %s%s\n", directory, dent->d_name);

                if (dent->d_reclen == 0)
                    break;
                nread -= dent->d_reclen;
                if (nread < 0)
                    break;
                dent = (struct dirent*)((char*)dent + dent->d_reclen);
            }
            PS2::memset(buffer, 0, sizeof(buffer));
        }

        PS::close(fd);
    }

    // Disconnect from debug server
    PS::Debug.disconnect();

    // Restore corruption
    PS::Breakout::restore();
}