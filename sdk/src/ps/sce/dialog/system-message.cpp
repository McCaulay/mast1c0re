#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/dialog/system-message.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>

PS::Sce::MsgDialogSystemMessage::MsgDialogSystemMessage() : PS::Sce::MsgDialog::MsgDialog(PS::Sce::MsgDialog::Mode::SYSTEM_MSG)
{
    // PS2::memset(&this->param, 0x0, sizeof(PS::Sce::MsgDialogSystemMessage::Param));
    // this->setSystemMessageParam(VAR_TO_NATIVE(this->param));
}
#endif