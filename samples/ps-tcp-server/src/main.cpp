#include <mast1c0re.hpp>

#define SERVER_PORT 9030

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    #if (defined(PS4) && PS4)
    const char* system = "PS4";
    #elif (defined(PS5) && PS5)
    const char* system = "PS5";
    #endif

    // Create server
    PS::TcpServer server = PS::TcpServer();

    // Listen
    if (server.listen(SERVER_PORT))
    {
        PS::notification("Listening on port %i", SERVER_PORT);

        // Accept connection
        PS::TcpClient client = server.accept();

        // Send message
        client.printf("Hello from %s on firmware %i!\n", system, FIRMWARE);

        // Disconnect
        client.disconnect();
        server.disconnect();
    }
    else
        PS::notification("Failed to listen on port %i", SERVER_PORT);

    // Restore corruption
    PS::Breakout::restore();
}