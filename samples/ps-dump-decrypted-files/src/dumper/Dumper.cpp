#include "Dumper.hpp"
#include "../elf/Header.hpp"
#include "../elf/Program.hpp"

bool Dumper::dumpDirectory(PS::TcpClient* client, const char* dirpath)
{
    PS::Debug.printf("Dumping %s...\n", dirpath);

    // Open directory
    int fd = PS::open(dirpath, 0, 0);
    if (fd <= 0)
    {
        PS::Debug.printf("Error: Failed to open common/lib directory!\n");
        PS::Sce::MsgDialogUserMessage::show("Failed to open common/lib directory!");
        return false;
    }

    // Iterate directories/files
    char buffer[8192];
    int nread = 0;
    while ((nread = PS::getdents(fd, buffer, sizeof(buffer))) > 0)
    {
        struct dirent* dent = (struct dirent*)buffer;
        while (dent->d_fileno)
        {
            if (dent->d_type == DT_REG)
                Dumper::dumpFile(client, dirpath, dent->d_name);

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
    return true;
}

bool Dumper::dumpFile(PS::TcpClient* client, const char* dirpath, const char* filename)
{
    char filepath[MAX_PATH];
    PS2::memset(filepath, 0, MAX_PATH);
    PS2::strcpy(filepath, dirpath);
    PS2::strcat(filepath, filename);

    int filenameLen = PS2::strlen(filename);

    // Send filename (tag/length/value)
    uint32_t PROTO_TAG_FILENAME = 0x41c11d9c;
    client->send(&PROTO_TAG_FILENAME, sizeof(PROTO_TAG_FILENAME));
    client->send(&filenameLen, sizeof(int));
    client->send((void*)filename, filenameLen);

    // Get filesize
    size_t filesize = PS::Filesystem::getFileSize(filepath);
    PS::Debug.printf("- %s (%llub)\n", filename, filesize);

    // Read file
    int32_t fd = PS::open(filepath, O_RDONLY, 0);
    if (fd <= 0)
    {
        PS::Debug.printf("  - Error: Failed to open %s\n", filepath);
        return false;
    }

    // Allocate file in memory
    uint64_t mapFile = PS::mmap(0, filesize, PROT_READ, MAP_SHARED, fd, 0);
    if (mapFile == MAP_FAILED)
    {
        PS::Debug.printf("  - Error: Failed to map file %s in memory\n", filepath);
        PS::close(fd);
        return false;
    }

    // Find ELF header
    uint64_t elf = Dumper::searchElfHeader(fd, mapFile, filesize);
    if (elf == UINT64_MAX)
    {
        PS::Debug.printf("  - Error: Failed to find ELF header in %s\n", filepath);
        PS::munmap(mapFile, filesize);
        PS::close(fd);
        return false;
    }

    // Read ELF header
    ElfHeader elfHeader;
    PS::Memory::read(elf, &elfHeader, sizeof(elfHeader));

    #if defined(LOG_EXTRA) && LOG_EXTRA
    PS::Debug.printf("  - ELF Header\n");
    PS::Debug.printf("    - e_type      = %04x\n", elfHeader.e_type);
    PS::Debug.printf("    - e_machine   = %04x\n", elfHeader.e_machine);
    PS::Debug.printf("    - e_version   = %08x\n", elfHeader.e_version);
    PS::Debug.printf("    - e_entry     = %08x%08x\n", (uint32_t)(elfHeader.e_entry >> 32), (uint32_t)elfHeader.e_entry);
    PS::Debug.printf("    - e_phoff     = %08x%08x\n", (uint32_t)(elfHeader.e_phoff >> 32), (uint32_t)elfHeader.e_phoff);
    PS::Debug.printf("    - e_shoff     = %08x%08x\n", (uint32_t)(elfHeader.e_shoff >> 32), (uint32_t)elfHeader.e_shoff);
    PS::Debug.printf("    - e_flags     = %08x\n", elfHeader.e_flags);
    PS::Debug.printf("    - e_ehsize    = %04x\n", elfHeader.e_ehsize);
    PS::Debug.printf("    - e_phentsize = %04x\n", elfHeader.e_phentsize);
    PS::Debug.printf("    - e_phnum     = %04x\n", elfHeader.e_phnum);
    PS::Debug.printf("    - e_shentsize = %04x\n", elfHeader.e_shentsize);
    PS::Debug.printf("    - e_shnum     = %04x\n", elfHeader.e_shnum);
    PS::Debug.printf("    - e_shstrndx  = %04x\n", elfHeader.e_shstrndx);
    #endif

    // Read ELF program header
    ElfProgram elfPrograms[128];
    if (elfHeader.e_phnum > 128)
    {
        PS::Debug.printf("  - Error: Maximum of 128 program header supported. Found %i\n", elfHeader.e_phnum);
        PS::munmap(mapFile, filesize);
        PS::close(fd);
        return false;
    }
    PS::Memory::read(elf + elfHeader.e_phoff, &elfPrograms, sizeof(ElfProgram) * elfHeader.e_phnum);

    // Send ELF headers
    uint64_t headerSize = elfHeader.e_phoff + (sizeof(ElfProgram) * elfHeader.e_phnum);
    if (!Dumper::sendFileData(client, elf, headerSize, 0))
    {
        PS::Debug.printf("  - Failed to send ELF header %s\n", filepath);
        PS::munmap(mapFile, filesize);
        PS::close(fd);
        return false;
    }

    // Loop each program header
    for (uint32_t i = 0; i < elfHeader.e_phnum; i++)
    {
        ElfProgram program = elfPrograms[i];

        // Ignore empty segments
        if (program.p_filesz == 0)
        {
            #if defined(LOG_EXTRA) && LOG_EXTRA
            PS::Debug.printf("    - Map program #%i empty, ignoring\n", i);
            #endif
            continue;
        }

        #if defined(LOG_EXTRA) && LOG_EXTRA
        PS::Debug.printf("    - ELF Program #%i\n", i);
        PS::Debug.printf("      - p_type      = %04x\n", program.p_type);
        PS::Debug.printf("      - p_flags     = %04x\n", program.p_flags);
        PS::Debug.printf("      - p_offset    = %08x%08x\n", (uint32_t)(program.p_offset >> 32), (uint32_t)program.p_offset);
        PS::Debug.printf("      - p_vaddr     = %08x%08x\n", (uint32_t)(program.p_vaddr >> 32), (uint32_t)program.p_vaddr);
        PS::Debug.printf("      - p_paddr     = %08x%08x\n", (uint32_t)(program.p_paddr >> 32), (uint32_t)program.p_paddr);
        PS::Debug.printf("      - p_filesz    = %08x%08x\n", (uint32_t)(program.p_filesz >> 32), (uint32_t)program.p_filesz);
        PS::Debug.printf("      - p_memsz     = %08x%08x\n", (uint32_t)(program.p_memsz >> 32), (uint32_t)program.p_memsz);
        PS::Debug.printf("      - p_align     = %08x%08x\n", (uint32_t)(program.p_align >> 32), (uint32_t)program.p_align);
        #endif

        // Map segment to memory
        uint64_t mapProgram = PS::mmap(0, program.p_filesz, PROT_READ, MAP_SHARED|0x80000, fd, (uint64_t)i << 32);
        if (mapProgram == MAP_FAILED)
        {
            #if defined(LOG_EXTRA) && LOG_EXTRA
            PS::Debug.printf("    - Failed to map program #%i in memory, ignoring\n", i);
            #endif
            continue;
        }

        // Check we can access memory
        #ifdef SYSCALL_SUPPORT
        if (PS::madvise(mapProgram, program.p_filesz, MADV_WILLNEED) != 0)
        {
            #if defined(LOG_EXTRA) && LOG_EXTRA
            PS::Debug.printf("    - madvise failed for segment #%i, ignoring\n", i);
            #endif
            PS::munmap(mapFile, filesize);
            continue;
        }
        #endif

        // Send file contents
        if (!Dumper::sendFileData(client, mapProgram, program.p_filesz, program.p_offset))
        {
            PS::Debug.printf("    - Failed to send %s program #%i\n", filepath, i);
            PS::munmap(mapProgram, program.p_filesz);
            PS::munmap(mapFile, filesize);
            PS::close(fd);
            return false;
        }

        PS::munmap(mapProgram, program.p_filesz);
    }

    // Free map file
    PS::munmap(mapFile, filesize);
    PS::close(fd);
    return true;
}

bool Dumper::sendFileData(PS::TcpClient* client, uint64_t address, uint64_t size, uint64_t offset)
{
    uint32_t PROTO_TAG_FILEDATA = 0x55e58570;
    client->send(&PROTO_TAG_FILEDATA, sizeof(PROTO_TAG_FILEDATA));
    client->send(&offset, sizeof(offset));
    client->send(&size, sizeof(size));
    return client->sendNative(address, size) == size;
}

uint64_t Dumper::searchElfHeader(int fd, uint64_t address, uint64_t maxOffset)
{
    for (uint64_t offset = 0; offset < maxOffset; offset++)
    {
        PS::Sce::Kernel::Lseek(fd, offset, SEEK_SET);
        if (PS::read<uint32_t>(fd) == 0x464c457f) // ELF\x7F
        {
            PS::Sce::Kernel::Lseek(fd, 0, SEEK_SET);
            return address + offset;
        }
    }
    return UINT64_MAX;
}