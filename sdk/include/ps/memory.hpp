#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/breakout.hpp>
#include <ps/ps.hpp>

namespace PS
{
    class Memory
    {
    public:
        static uint16_t swapEndian(uint16_t value);
        static uint32_t swapEndian(uint32_t value);
        static uint64_t swapEndian(uint64_t value);

        template<typename T>
        static void write(uint64_t address, T value)
        {
            PS::memcpy(address, VAR_TO_NATIVE(value), sizeof(T));
        }

        template<typename T>
        static void write(uint64_t address, uint32_t index, T value)
        {
            write(address + (sizeof(T) * index), value);
        }

        template<typename T>
        static T read(uint64_t address)
        {
            T value;
            PS::memcpy(VAR_TO_NATIVE(value), address, sizeof(T));
            return value;
        }

        template<typename T>
        static T read(uint64_t address, uint32_t index)
        {
            return read<T>(address + (sizeof(T) * index));
        }

        static void write(uint64_t address, void* buffer, size_t n);
        static void* read(uint64_t address, void* buffer, size_t n);

        static char* readString(uint64_t address);

        static void writeStdString(uint64_t address, const char* s);
        static char* readStdString(uint64_t address);
    public:
        Memory(uint64_t address);
        uint64_t getAddress();
        PS::Memory* dereference();
        PS::Memory* move(int64_t offset);

        template<typename T>
        void write(T value)
        {
            PS::Memory::write<T>(this->address, value);
        }

        template<typename T>
        void write(uint32_t index, T value)
        {
            PS::Memory::write<T>(this->address + (sizeof(T) * index), value);
        }

        template<typename T>
        T read()
        {
            return PS::Memory::read<T>(this->address);
        }

        template<typename T>
        T read(uint32_t index)
        {
            return PS::Memory::read<T>(this->address + (sizeof(T) * index));
        }

        char* readString();

        void writeStdString(const char* s);
        char* readStdString();
    private:
        uint64_t address;
    };
}
#endif