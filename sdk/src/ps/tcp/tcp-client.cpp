#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/tcp/tcp-client.hpp>
#include <ps2/ps2.hpp>
#include <ps/ps.hpp>
#include <ps/syscall/syscall.hpp>
#include <stdarg.h>

#ifdef LIBKERNEL
PS::TcpClient::TcpClient()
{
    this->sock = 0;
    this->connected = false;
}

PS::TcpClient::TcpClient(int32_t sock, struct sockaddr_in remote)
{
    this->sock = sock;
    this->remote = remote;
    this->connected = true;
}

bool PS::TcpClient::connect(uint32_t ip, uint16_t port)
{
    if (this->isConnected())
        return true;

    this->sock = 0;

    this->remote.sin_len = sizeof(this->remote);
    this->remote.sin_family = AF_INET;
    this->remote.sin_addr.s_addr = ip;
    this->remote.sin_port = PS::htons(port);
    PS2::memset(this->remote.sin_zero, 0, sizeof(this->remote.sin_zero));

    int32_t sock = PS::socket(AF_INET, SOCK_STREAM, 0);
    if (sock >= 0 && PS::connect(sock, (struct sockaddr *)&this->remote, sizeof(this->remote)) == 0)
    {
        this->sock = sock;
        this->connected = true;
        return true;
    }
    this->connected = false;
    return false;
}

bool PS::TcpClient::isConnected()
{
    return this->connected && this->sock > 0;
}

size_t PS::TcpClient::read(void* buffer, size_t length)
{
    if (!this->isConnected())
        return 0;

    return PS::readAll(this->sock, buffer, length);
}

size_t PS::TcpClient::send(void* buffer, size_t length)
{
    if (!this->isConnected())
        return 0;

    return PS::writeAll(this->sock, buffer, length);
}

size_t PS::TcpClient::printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char message[8192];
    PS2::vsprintf(message, format, args);
    va_end(args);

    return this->send(message, PS2::strlen(message));
}

size_t PS::TcpClient::memory(uint8_t* buffer, size_t length)
{
    int64_t remaining = (int64_t)length;
    size_t count = 0;
    size_t i = 0;
    for (; remaining >= 16; i += 16, remaining -= 16)
        count += PS::TcpClient::memoryLine(buffer, i, 16, 16);

    if (remaining > 0)
        count += PS::TcpClient::memoryLine(buffer, i, remaining, 16);
    return count;
}

size_t PS::TcpClient::memoryLine(uint8_t* base, size_t baseOffset, size_t count, size_t width)
{
    uint8_t* data = base + baseOffset;

    size_t sent = this->printf("| %04x: ", (uint32_t)baseOffset);
    for (size_t i = 0; i < width; i++)
    {
        if (i < count)
            sent += this->printf("%02x ", data[i]);
        else
            sent += this->printf("   ");
    }
    sent += this->printf("| ");
    for (size_t i = 0; i < width; i++)
    {
        char c = '.';
        if (i >= count)
            c = ' ';
        else if ((char)data[i] >= 0x20 && (char)data[i] <= 0x7E)
            c = (char)data[i];
        sent += this->printf("%c", c);
    }
    sent += this->printf(" |\n");
    return sent;
}

void PS::TcpClient::disconnect()
{
    if (!this->isConnected())
        return;

	PS::close(this->sock);
    this->sock = 0;
    this->connected = false;
}
#endif
#endif