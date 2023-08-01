#pragma once

#include <types.hpp>

#define PT_NULL    0
#define PT_LOAD    1

#pragma pack(push, 1)
class ElfProgram
{
public:
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};
#pragma pack(pop)