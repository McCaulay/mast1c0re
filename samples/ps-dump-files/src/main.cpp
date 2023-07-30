#include <mast1c0re.hpp>
#include "dumper/Dumper.hpp"

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Attempt to connect to debug server
    PS::Debug.connect(IP(192, 168, 0, 7), 9023);

    // Connect to remote server
    PS::TcpClient client = PS::TcpClient();
    if (client.connect(IP(192, 168, 0, 7), 9031))
    {
        // Signal start
        uint32_t PROTO_TAG_START = 0x6e92f11b;
        client.send(&PROTO_TAG_START, sizeof(PROTO_TAG_START));

        // Get randomized path
        char randomizedPath[11];
        PS2::memset(randomizedPath, 0, sizeof(randomizedPath));
        int randomizedPathLen = sizeof(randomizedPath);
        if (PS::Sce::Kernel::RandomizedPath(randomizedPath, &randomizedPathLen) == 0)
        {
            // Build library path
            char libraryPath[MAX_PATH];
            PS2::memset(libraryPath, 0, MAX_PATH);
            libraryPath[0] = '/';
            PS2::strcat(libraryPath, randomizedPath);
            PS2::strcat(libraryPath, "/common/lib/");

            // Dump libraries to server
            Dumper::dumpDirectory(&client, libraryPath);
        }
        else
            PS::Sce::MsgDialogUserMessage::show("Failed to get randomized path!");

        // Disconnect
        uint32_t PROTO_TAG_END = 0xdabea3b6;
        client.send(&PROTO_TAG_END, sizeof(PROTO_TAG_END));
        client.disconnect();
    }
    else
        PS::Sce::MsgDialogUserMessage::show("Failed to connect to server!");

    // Disconnect from debug server
    PS::Debug.disconnect();

    // Restore corruption
    PS::Breakout::restore();
}