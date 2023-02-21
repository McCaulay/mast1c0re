#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/tcp/tcp-server.hpp>
#include <ps2/ps2.hpp>
#include <ps/ps.hpp>
#include <ps/syscall/syscall.hpp>
#include <stdarg.h>

#ifdef LIBKERNEL
PS::TcpServer::TcpServer()
{
    this->sock = 0;
}

bool PS::TcpServer::listen(uint16_t port, int32_t backlog)
{
    this->sock = 0;

    this->server.sin_len = sizeof(this->server);
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = IN_ADDR_ANY;
    this->server.sin_port = PS::htons(port);
    PS2::memset(this->server.sin_zero, 0, sizeof(this->server.sin_zero));

    // Create socket
    int32_t sock = PS::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return false;
    this->sock = sock;

    // Bind socket
    if (PS::bind(this->sock, (struct sockaddr *)&this->server, sizeof(this->server)) < 0)
        return false;

    // Start listening
    if (PS::listen(sock, backlog) < 0)
        return false;

    return true;
}

PS::TcpClient PS::TcpServer::accept()
{
    struct sockaddr_in userAddr;
    uint32_t length = sizeof(userAddr);
    int32_t userSock = PS::accept(this->sock, (struct sockaddr *)&userAddr, &length);
    return PS::TcpClient(userSock, userAddr);
}

void PS::TcpServer::disconnect()
{
    if (this->sock == 0)
        return;

	PS::close(this->sock);
    this->sock = 0;
}
#endif
#endif