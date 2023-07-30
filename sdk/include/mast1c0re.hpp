#pragma once

#include <types.hpp>
#include <compiler.hpp>
#include <arith64.hpp>
#include <cstdint>
#include <ps2/ps2.hpp>
#include <offsets/okage.hpp>
#include <ps2/okage.hpp>
#include <ps2/cdvd.hpp>
#include <common/list.hpp>

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
    #include <offsets/ps/eboot/eboot.hpp>
    #include <offsets/ps/libkernel.hpp>
    #include <offsets/ps/sce/msg-dialog.hpp>
    #include <ps/syscall/table.hpp>
    #include <ps/breakout.hpp>
    #include <ps/ps.hpp>
    #include <ps/memory.hpp>
    #include <ps/sce/sce.hpp>
    #include <ps/sce/libkernel.hpp>
    #include <ps/syscall/syscall.hpp>
    #include <ps/filesystem/filesystem.hpp>
    #include <ps/syscall/socket.hpp>
    #include <ps/tcp/tcp-client.hpp>
    #include <ps/tcp/tcp-server.hpp>
    #include <ps/sce/dialog/progress-bar.hpp>
    #include <ps/sce/dialog/user-message.hpp>
    #include <ps/sce/dialog/system-message.hpp>
#endif

#if (defined(PS4) && PS4)
    #include <ps/sce/usb/usb/usb.hpp>
#endif