#include <mast1c0re.hpp>

void main()
{
    Okage::clearScreenText();

    #if (defined(PS4) && PS4)
    const char* system = "PS4";
    #elif (defined(PS5) && PS5)
    const char* system = "PS5";
    #elif (defined(PCSX2) && PCSX2)
    const char* system = "PCSX2";
    #endif

    Okage::printf("Hello %s!\\n", system);
}