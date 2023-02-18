#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Show notification with Platinum Trophy
    PS::notificationWithIcon("cxml://psnotification/tex_morpheus_trophy_platinum", "PS2 Breakout");

    // Show Process Id
    PS::notification("Process Id: %i", PS::getpid());

    // Restore corruption
    PS::Breakout::restore();
}