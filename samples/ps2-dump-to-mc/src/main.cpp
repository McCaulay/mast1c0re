#include <mast1c0re.hpp>

#define OKAGE_LOVE_FLAG 0x225290

void main()
{
    Okage::clearScreenText();

    const char* dumpFile = "BASCUS-97129/dump.bin";
    char* dumpAddress = (char*)OKAGE_LOVE_FLAG;

    // Dump memory to MC
    int fd = Okage::MemoryCardOpen(dumpFile, 1539);
    if (fd < 0)
    {
        Okage::printf("Failed to open %s.\\n", dumpFile);
        return;
    }

    Okage::MemoryCardWrite(fd, dumpAddress, PS2::strlen(dumpAddress));
    Okage::MemoryCardClose(fd);
    Okage::printf("\"%s\" dumped to %s\\n", dumpAddress, dumpFile);
}