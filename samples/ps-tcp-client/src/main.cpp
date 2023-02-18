#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    #if (defined(PS4) && PS4)
    const char* system = "PS4";
    #elif (defined(PS5) && PS5)
    const char* system = "PS5";
    #endif

    PS::TcpClient client = PS::TcpClient();

    // Connect to remote server
    client.connect(IP(192, 168, 0, 7), 9030);

    // Send message
    client.printf("Hello from %s on firmware %i!\n", system, FIRMWARE);

    // Disconnect
    client.disconnect();

    // Restore corruption
    PS::Breakout::restore();
}