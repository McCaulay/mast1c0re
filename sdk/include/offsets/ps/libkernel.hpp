#pragma once

// libkernel_sys.sprx

#if defined(PS4) && PS4
    #define LIB_KERNEL_SYS_RET_ERROR 0xc6

    #if defined(FIRMWARE) && FIRMWARE == 505
        #include <offsets/ps/libkernel/ps4/5.05.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 672
        #include <offsets/ps/libkernel/ps4/6.72.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 750 && FIRMWARE <= 755
        #include <offsets/ps/libkernel/ps4/7.50_7.55.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 800 && FIRMWARE <= 801
        #include <offsets/ps/libkernel/ps4/8.00_8.01.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 850
        #include <offsets/ps/libkernel/ps4/8.50.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 852
        #include <offsets/ps/libkernel/ps4/8.52.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 900 && FIRMWARE <= 904
        #include <offsets/ps/libkernel/ps4/9.00_9.04.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 950 && FIRMWARE <= 960
        #include <offsets/ps/libkernel/ps4/9.50_9.60.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 1000 && FIRMWARE <= 1001
        #include <offsets/ps/libkernel/ps4/10.00_10.01.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 1050 && FIRMWARE <= 1071
        #include <offsets/ps/libkernel/ps4/10.50_10.71.hpp>
    #elif defined(FIRMWARE) && FIRMWARE >= 1100 && FIRMWARE <= 1102
        #include <offsets/ps/libkernel/ps4/11.00_11.02.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 1150
        #include <offsets/ps/libkernel/ps4/11.50.hpp>
    #endif

    #if defined(LIB_KERNEL_SYS_RET_ERROR) && defined(FIRMWARE) && FIRMWARE < 1001
        #define SYSCALL_SUPPORT true
    #endif
#elif defined(PS5) && PS5
    #if defined(FIRMWARE) && FIRMWARE >= 300 && FIRMWARE <= 321
        #define LIB_KERNEL_SYS_RET_ERROR 0x35b6
    #elif defined(FIRMWARE) && FIRMWARE >= 400 && FIRMWARE <= 451
        #define LIB_KERNEL_SYS_RET_ERROR 0x35d6
    #elif defined(FIRMWARE) && FIRMWARE >= 650 && FIRMWARE <= 900
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
