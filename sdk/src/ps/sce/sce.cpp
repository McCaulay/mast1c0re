#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/sce.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>

int32_t PS::Sce::PadSetLightBar(int32_t handle, int32_t* colour)
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_PAD_SET_LIGHTBAR_STUB), handle, PVAR_TO_NATIVE(colour));
}
#endif