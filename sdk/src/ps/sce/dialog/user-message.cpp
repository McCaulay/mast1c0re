#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/dialog/user-message.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>

bool PS::Sce::MsgDialogUserMessage::show(const char* message, PS::Sce::MsgDialog::ButtonType type)
{
    PS::Sce::MsgDialog::Initialize();
    PS::Sce::MsgDialogUserMessage dialog = PS::Sce::MsgDialogUserMessage(message, type);
    dialog.open();

    // Wait for dialog to close
    while (dialog.updateStatus() != PS::Sce::CommonDialog::Status::FINISHED);

    dialog.close();

    // Get dialog result
    PS::Sce::MsgDialog::Result result;
    PS2::memset(&result, 0, sizeof(PS::Sce::MsgDialog::Result));
    dialog.getResult(&result);

    PS::Sce::MsgDialog::Terminate();
    return result.buttonId == PS::Sce::MsgDialog::ButtonId::OK_YES;
}

PS::Sce::MsgDialogUserMessage::MsgDialogUserMessage(const char* message, PS::Sce::MsgDialog::ButtonType type) : PS::Sce::MsgDialog::MsgDialog(PS::Sce::MsgDialog::Mode::USER_MSG)
{
    PS2::memset(&this->param, 0x0, sizeof(PS::Sce::MsgDialogUserMessage::Param));
    this->param.buttonType = type;
    this->param.msg = PVAR_TO_NATIVE(message);
    this->setUserMessageParam(VAR_TO_NATIVE(this->param));
}
#endif