#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/syscall/socket.hpp>
#include <ps/sce/libkernel.hpp>

#ifdef LIBKERNEL
namespace PS
{
    class TcpClient
    {
    public:
        TcpClient();
        TcpClient(int32_t sock, struct sockaddr_in remote);
        bool connect(uint32_t ip, uint16_t port);
        bool isConnected();

        template<typename T>
        size_t send(T value)
        {
            return this->send(&value, sizeof(T));
        }

        template<typename T>
        T read()
        {
            T value;
            this->read(&value, sizeof(T));
            return value;
        }
        
        size_t read(void* buffer, size_t length);
        size_t send(void* buffer, size_t length);
        size_t printf(const char* format, ...);
        size_t memory(uint8_t* buffer, size_t length);
        void disconnect();
    private:
        size_t memoryLine(uint8_t* base, size_t baseOffset, size_t count, size_t width);
    private:
        int32_t sock;
        struct sockaddr_in remote;
        bool connected;
    };
}
#endif
#endif