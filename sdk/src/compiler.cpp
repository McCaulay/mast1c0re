#include <compiler.hpp>
#include <cstdint>
#include <ps2/ps2.hpp>

extern "C"
{
    void* memset(void* str, int c, int n)
    {
        for (int i = 0; i < n; i++)
            *((uint8_t*)str + i) = (uint8_t)c;
        return str;
    }

    void* memcpy(void* dest, void* src, unsigned int n)
    {
        return PS2::memcpy(dest, src, n);
    }
}