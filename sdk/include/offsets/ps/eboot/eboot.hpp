#pragma once

// EBOOT.BIN

#if ((defined(PS4) && PS4) || (defined(PS5) && PS5)) && defined(EBOOT_VERSION)
    #if EBOOT_VERSION == 100
        #include <offsets/ps/eboot/1.00.hpp>
        #include <offsets/ps/eboot/gadgets/1.00.hpp>
    #elif EBOOT_VERSION == 101
        #include <offsets/ps/eboot/1.01.hpp>
        #include <offsets/ps/eboot/gadgets/1.01.hpp>
    #endif
#endif