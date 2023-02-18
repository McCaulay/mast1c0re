#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/dialog/progress-bar.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>
#include <offsets/ps/sce/msg-dialog.hpp>

int32_t PS::Sce::MsgDialogProgressBar::Inc(SceMsgDialogProgressBarTarget target, uint32_t delta)
{
    return (int32_t)PS::Breakout::call(DEREF(EBOOT(EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB_PTR)) + SCE_MSG_DIALOG_PROGRESS_BAR_INC_OFFSET_TERMINATE, target, delta);
}

int32_t PS::Sce::MsgDialogProgressBar::SetMsg(SceMsgDialogProgressBarTarget target, const char *barMsg)
{
    return (int32_t)PS::Breakout::call(DEREF(EBOOT(EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB_PTR)) + SCE_MSG_DIALOG_PROGRESS_BAR_SET_MSG_OFFSET_TERMINATE, target, PVAR_TO_NATIVE(barMsg));
}

int32_t PS::Sce::MsgDialogProgressBar::SetValue(SceMsgDialogProgressBarTarget target, uint32_t rate)
{
    return (int32_t)PS::Breakout::call(DEREF(EBOOT(EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB_PTR)) + SCE_MSG_DIALOG_PROGRESS_BAR_SET_VALUE_OFFSET_TERMINATE, target, rate);
}

PS::Sce::MsgDialogProgressBar::MsgDialogProgressBar(const char* message, PS::Sce::MsgDialogProgressBar::Type type) : PS::Sce::MsgDialog::MsgDialog(PS::Sce::MsgDialog::Mode::PROGRESS_BAR)
{
    PS2::memset(&this->param, 0x0, sizeof(PS::Sce::MsgDialogProgressBar::Param));
    this->param.barType = type;
    this->param.msg = PVAR_TO_NATIVE(message);
    this->setProgressBarParam(VAR_TO_NATIVE(this->param));
}

int32_t PS::Sce::MsgDialogProgressBar::inc(uint32_t delta)
{
    return PS::Sce::MsgDialogProgressBar::Inc(0, delta);
}

int32_t PS::Sce::MsgDialogProgressBar::setMsg(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char message[8192];
    PS2::vsprintf(message, format, args);
    va_end(args);

    return PS::Sce::MsgDialogProgressBar::SetMsg(0, message);
}

int32_t PS::Sce::MsgDialogProgressBar::setValue(uint32_t rate)
{
    return PS::Sce::MsgDialogProgressBar::SetValue(0, rate);
}
#endif