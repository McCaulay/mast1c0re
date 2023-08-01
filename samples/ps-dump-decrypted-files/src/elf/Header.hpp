#pragma once

#include <types.hpp>

#define EI_MAG0         0  // File identification
#define EI_MAG1         1  // File identification
#define EI_MAG2         2  // File identification
#define EI_MAG3         3  // File identification
#define EI_CLASS        4  // File class
#define EI_DATA         5  // Data encoding
#define EI_VERSION      6  // File version
#define EI_PAD          7  // Start of padding bytes
#define EI_NIDENT       16 // Size of e_ident[]

#define ELFMAG0         0x7f // e_ident[EI_MAG0]
#define ELFMAG1         'E'  // e_ident[EI_MAG1]
#define ELFMAG2         'L'  // e_ident[EI_MAG2]
#define ELFMAG3         'F'  // e_ident[EI_MAG3]
#define ELFCLASS32      1 // 32-bit objects
#define ELFDATA2LSB     1

#define ET_EXEC         2 // Executable file
#define EM_MIPS         8 // MIPS
#define EV_CURRENT      1 // Current version

#pragma pack(push, 1)
class ElfHeader
{
public:
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    uint64_t      e_entry;
    uint64_t      e_phoff;
    uint64_t      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
};
#pragma pack(pop)