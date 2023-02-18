#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Attempt to connect to debug server
    PS::Debug.connect(IP(192, 168, 0, 7), 9023);

    // Send debug messages, ignored if not connected to server
    PS::Debug.printf("Hello World\n");
    PS::Debug.printf("Process Id: %i\n", PS::getpid());

    // Disconnect from debug server
    PS::Debug.disconnect();

    // Restore corruption
    PS::Breakout::restore();
}