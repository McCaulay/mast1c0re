#include <mast1c0re.hpp>
#include "downloader/Downloader.hpp"

#define SERVER_PORT 9045

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Set pad light to purple
    PS::PadSetLightBar(150, 0, 255, 255);

    // Show "PS2 Game Loader" notification
    PS::notificationWithIcon("cxml://psnotification/tex_morpheus_trophy_platinum", "PS2 Game Loader (Network)");

    // Attempt to connect to debug server
    PS::Debug.connect(IP(192, 168, 0, 7), 9023);
    PS::Debug.printf("---------- Load PS2 Game (Network) ----------\n");

    // Download ISO if doesn't exist
    const char* filepath = "/av_contents/content_tmp/disc01.iso";
    if (!PS::Filesystem::exists(filepath))
    {
        // Download ISO
        if (!Downloader::download(filepath, SERVER_PORT))
        {
            PS::Debug.printf("Failed to download ISO\n");

            // Disconnect from debug server
            PS::Debug.disconnect();

            // Restore corruption
            PS::Breakout::restore();
            return;
        }
    }

    // Mount & Load iso
    PS::Debug.printf("Mounting...\n");
    PS::MountDiscWithFilepath("./../av_contents/content_tmp/disc01.iso");

    // Get game code from mounted file
    char* gameCode = PS::GetMountedGameCode();
    if (PS2::strlen(gameCode) == 10)
    {
        // Convert name from "SCUS-97129" -> "cdrom0:\\SCUS_971.29;1"
        char* ps2Path = PS2::gameCodeToPath(gameCode);

        // Disconnect from debug server
        PS::Debug.printf("Loading \"%s\"...\n", ps2Path);

        // Disconnect from debug server
        PS::Debug.disconnect();

        // Restore corruption
        PS::Breakout::restore();

        // Execute mounted iso
        PS2::LoadExecPS2(ps2Path, 0, NULL);
        return;
    }

    PS::Debug.printf("Unexpected game code (%s) length of %i, expecting %i\n", gameCode, PS2::strlen(gameCode), 10);

    // Disconnect from debug server
    PS::Debug.disconnect();

    // Restore corruption
    PS::Breakout::restore();
}