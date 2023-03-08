#pragma once

// libkernel_sys.sprx

#if defined(PS4) && PS4
    #if defined(FIRMWARE) && FIRMWARE == 505
        #include <offsets/ps/libkernel/ps4/5.05.hpp>
        #define LIB_KERNEL_SYS_RET 0x2b9a
    #elif defined(FIRMWARE) && FIRMWARE == 672
        #include <offsets/ps/libkernel/ps4/6.72.hpp>
        #define LIB_KERNEL_SYS_RET 0x2c4a
    #elif defined(FIRMWARE) && FIRMWARE == 900
        #include <offsets/ps/libkernel/ps4/9.00.hpp>
        #define LIB_KERNEL_SYS_RET 0x2baa
    #elif defined(FIRMWARE) && FIRMWARE == 1001
        #include <offsets/ps/libkernel/ps4/10.01.hpp>
    #elif defined(FIRMWARE) && FIRMWARE == 1050
        #include <offsets/ps/libkernel/ps4/10.50.hpp>
    #endif
#elif defined(PS5) && PS5
    #if defined(FIRMWARE) && FIRMWARE == 650
        #include <offsets/ps/libkernel/ps5/6.50.hpp>
        #define LIB_KERNEL_SYS_RET 0x038a
    #endif
#endif