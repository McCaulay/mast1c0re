#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/syscall/socket.hpp>
#include <ps/breakout.hpp>

uint32_t PS::htonl(uint32_t hostlong)
{
    return (
        ((hostlong & 0xFF000000) >> 24) |
        ((hostlong & 0x00FF0000) >> 8) |
        ((hostlong & 0x0000FF00) << 8) |
        ((hostlong & 0x000000FF) << 24)
    );
}

uint16_t PS::htons(uint16_t hostshort)
{
    return ((hostshort >> 8) | (hostshort << 8));
}

uint32_t PS::ntohl(uint32_t netlong)
{
    return PS::htonl(netlong);
}

uint16_t PS::ntohs(uint16_t netshort)
{
    return PS::htons(netshort);
}

#ifdef LIB_KERNEL_LEAKED
int32_t PS::accept(int32_t sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
    return (int32_t)PS::Breakout::syscall(30, sockfd, PVAR_TO_NATIVE(addr), PVAR_TO_NATIVE(addrlen));
}

int32_t PS::bind(int32_t sockfd, struct sockaddr* addr, socklen_t addrlen)
{
    return (int32_t)PS::Breakout::syscall(104, sockfd, PVAR_TO_NATIVE(addr), addrlen);
}

int32_t	PS::connect(int32_t sockfd, const struct sockaddr *addr, socklen_t addrLen)
{
    return (int32_t)PS::Breakout::syscall(98, sockfd, PVAR_TO_NATIVE(addr), addrLen);
}

int32_t PS::listen(int32_t sockfd, int32_t backlog)
{
    return (int32_t)PS::Breakout::syscall(106, sockfd, backlog);
}

int32_t PS::socket(int32_t domain, int32_t type, int32_t protocol)
{
    return (int32_t)PS::Breakout::syscall(97, domain, type, protocol);
}
#endif
#endif