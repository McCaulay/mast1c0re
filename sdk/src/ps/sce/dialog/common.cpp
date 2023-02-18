#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/dialog/common.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>

bool PS::Sce::CommonDialog::isInitialized = false;

int32_t PS::Sce::CommonDialog::Initialize()
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_COMMON_DIALOG_INITIALIZE_STUB));
}

bool PS::Sce::CommonDialog::IsUsed()
{
    return (bool)PS::Breakout::call(EBOOT(EBOOT_SCE_COMMON_DIALOG_IS_USED_STUB));
}

PS::Sce::CommonDialog::CommonDialog()
{
    if (!PS::Sce::CommonDialog::isInitialized)
    {
        PS::Sce::CommonDialog::Initialize();
        PS::Sce::CommonDialog::isInitialized = true;
    }
}
#endif