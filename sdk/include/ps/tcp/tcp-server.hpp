#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/syscall/socket.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps/tcp/tcp-client.hpp>

#if defined(LIBKERNEL) || defined(LIB_KERNEL_SYS_RET_ERROR)
namespace PS
{
    class TcpServer
    {
    public:
        TcpServer();
        bool listen(uint16_t port, int32_t backlog = 1);
        PS::TcpClient accept();
        void disconnect();
    private:
        int32_t sock;
        struct sockaddr_in server;
    };
}
#endif
#endif