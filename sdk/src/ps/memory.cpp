#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/memory.hpp>
#include <ps2/ps2.hpp>
#include <ps/sce/sce.hpp>

uint16_t PS::Memory::swapEndian(uint16_t value)
{
    return ((value >> 8) | (value << 8));
}

uint32_t PS::Memory::swapEndian(uint32_t value)
{
    return (
        ((value & 0xFF000000) >> 24) |
        ((value & 0x00FF0000) >> 8) |
        ((value & 0x0000FF00) << 8) |
        ((value & 0x000000FF) << 24)
    );
}

uint64_t PS::Memory::swapEndian(uint64_t value)
{
    value = ((value & 0x00000000FFFFFFFF) << 32) | ((value & 0xFFFFFFFF00000000) >> 32);
    value = ((value & 0x0000FFFF0000FFFF) << 16) | ((value & 0xFFFF0000FFFF0000) >> 16);
    value = ((value & 0x00FF00FF00FF00FF) << 8)  | ((value & 0xFF00FF00FF00FF00) >> 8);
    return value;
}

void PS::Memory::write(uint64_t address, void* buffer, size_t n)
{
    PS::memcpy(address, PVAR_TO_NATIVE(buffer), n);
}

void* PS::Memory::read(uint64_t address, void* buffer, size_t n)
{
    PS::memcpy(PVAR_TO_NATIVE(buffer), address, n);
    return buffer;
}

char* PS::Memory::readString(uint64_t address)
{
    uint32_t bufferSize = 16;
    char* buffer = (char*)PS2::malloc(bufferSize);
    uint32_t offset = 0;

    while (true)
    {
        uint64_t chars = PS::Memory::read<uint64_t>(address + (uint64_t)offset);
        char* str = (char*)&chars;

        for (int i = 0; i < 8; i++)
        {
            char current = str[i];
            buffer[offset] = current;

            // NULL termination
            if (current == '\0')
                return buffer;

            offset++;

            // Increase size of buffer
            if (offset >= bufferSize)
            {
                bufferSize *= 2;
                buffer = (char*)PS2::realloc(buffer, bufferSize);
                if (buffer == NULL)
                    return NULL;
            }
        }
    }

    return buffer;
}

PS::Memory::Memory(uint64_t address)
{
    this->address = address;
}

PS::Memory* PS::Memory::dereference()
{
    this->address = PS::Memory::read<uint64_t>(this->address);
    return this;
}

PS::Memory* PS::Memory::move(int64_t offset)
{
    this->address += offset;
    return this;
}

char* PS::Memory::readString()
{
    return PS::Memory::readString(this->address);
}
#endif