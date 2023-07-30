#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/syscall/socket.hpp>
#include <ps/syscall/table.hpp>
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

#if defined(LIBKERNEL) || defined(LIB_KERNEL_SYS_RET_ERROR)
int32_t PS::accept(int32_t sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_ACCEPT), sockfd, PVAR_TO_NATIVE(addr), PVAR_TO_NATIVE(addrlen));
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_ACCEPT, sockfd, PVAR_TO_NATIVE(addr), PVAR_TO_NATIVE(addrlen));
    #endif
}

int32_t PS::bind(int32_t sockfd, struct sockaddr* addr, socklen_t addrlen)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_BIND), sockfd, PVAR_TO_NATIVE(addr), addrlen);
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_BIND, sockfd, PVAR_TO_NATIVE(addr), addrlen);
    #endif
}

int32_t	PS::connect(int32_t sockfd, const struct sockaddr *addr, socklen_t addrLen)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_CONNECT), sockfd, PVAR_TO_NATIVE(addr), addrLen);
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_CONNECT, sockfd, PVAR_TO_NATIVE(addr), addrLen);
    #endif
}

int32_t PS::getsockopt(int32_t sockfd, int32_t level, int32_t optname, void *optval, socklen_t *optlen)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL__GETSOCKOPT), sockfd, level, optname, PVAR_TO_NATIVE(optval), PVAR_TO_NATIVE(optlen));
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_GETSOCKOPT, sockfd, level, optname, PVAR_TO_NATIVE(optval), PVAR_TO_NATIVE(optlen));
    #endif
}

int32_t PS::listen(int32_t sockfd, int32_t backlog)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_LISTEN), sockfd, backlog);
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_LISTEN, sockfd, backlog);
    #endif
}

int32_t PS::setsockopt(int32_t sockfd, int32_t level, int32_t optname, void *optval, socklen_t optlen)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SETSOCKOPT), sockfd, level, optname, PVAR_TO_NATIVE(optval), optlen);
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_SETSOCKOPT, sockfd, level, optname, PVAR_TO_NATIVE(optval), optlen);
    #endif
}

int32_t PS::socket(int32_t domain, int32_t type, int32_t protocol)
{
    #if defined(LIBKERNEL)
    return (int32_t)PS::Breakout::call(LIBKERNEL(LIB_KERNEL_SOCKET), domain, type, protocol);
    #elif defined(LIB_KERNEL_SYS_RET_ERROR)
    return (int32_t)PS::Breakout::syscall(SYS_SOCKET, domain, type, protocol);
    #endif
}
#endif
#endif