#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    // Set pad light to orange
    PS::PadSetLightBar(250, 185, 0, 255);

    // Restore corruption
    PS::Breakout::restore();
}