#pragma once

// libkernel_sys.sprx

#if defined(PS4) && PS4
    #define LIB_KERNEL_SYS_RET_ERROR 0xc6

    #if defined(FIRMWARE) && FIRMWARE == 505
        #include <offsets/ps/libkernel/ps4/5.05.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 672
        #include <offsets/ps/libkernel/ps4/6.72.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 900
        #include <offsets/ps/libkernel/ps4/9.00.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 1001
        #include <offsets/ps/libkernel/ps4/10.01.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 1050 && FIRMWARE <= 1071
        #include <offsets/ps/libkernel/ps4/10.50_10.71.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 1100
        #include <offsets/ps/libkernel/ps4/11.00.hpp>
    #endif

    #if defined(LIB_KERNEL_SYS_RET_ERROR) && defined(FIRMWARE) && FIRMWARE < 1001
        #define SYSCALL_SUPPORT true
    #endif
#elif defined(PS5) && PS5
    #if defined(FIRMWARE) && FIRMWARE >= 300 && FIRMWARE <= 321
        #define LIB_KERNEL_SYS_RET_ERROR 0x35b6
    #elif defined(FIRMWARE) && FIRMWARE >= 400 && FIRMWARE <= 451
        #define LIB_KERNEL_SYS_RET_ERROR 0x35d6
    #elif defined(FIRMWARE) && FIRMWARE >= 650 && FIRMWARE <= 761
        #define LIB_KERNEL_SYS_RET_ERROR 0x35f6
    #endif

    #if defined(FIRMWARE) && FIRMWARE >= 300 && FIRMWARE <= 321
        #include <offsets/ps/libkernel/ps5/3.00_4.21.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 400 && FIRMWARE <= 403
        #include <offsets/ps/libkernel/ps5/4.00_4.03.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 450 && FIRMWARE <= 451
        #include <offsets/ps/libkernel/ps5/4.50_4.51.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 650
        #include <offsets/ps/libkernel/ps5/6.50.hpp>
    #endif

    #ifdef LIB_KERNEL_SYS_RET_ERROR
        #define SYSCALL_SUPPORT true
    #endif
#endif