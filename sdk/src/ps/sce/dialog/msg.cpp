#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/dialog/msg.hpp>
#include <ps/sce/libkernel.hpp>
#include <ps2/ps2.hpp>
#include <ps/breakout.hpp>
#include <offsets/ps/eboot/eboot.hpp>
#include <offsets/ps/sce/msg-dialog.hpp>

bool PS::Sce::MsgDialog::isInitialized = false;

int32_t PS::Sce::MsgDialog::Initialize()
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_MSG_DIALOG_INITIALIZE_STUB));
}

int32_t PS::Sce::MsgDialog::Open(PS::Sce::MsgDialog::Param *param)
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_MSG_DIALOG_OPEN_STUB), PVAR_TO_NATIVE(param));
}

int32_t PS::Sce::MsgDialog::GetResult(PS::Sce::MsgDialog::Result *result)
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB), PVAR_TO_NATIVE(result));
}

PS::Sce::CommonDialog::Status PS::Sce::MsgDialog::GetStatus()
{
    return (PS::Sce::CommonDialog::Status)PS::Breakout::call(DEREF(EBOOT(EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB_PTR)) + SCE_MSG_DIALOG_GET_STATUS_OFFSET_UPDATE);
}

PS::Sce::CommonDialog::Status PS::Sce::MsgDialog::UpdateStatus()
{
    return (PS::Sce::CommonDialog::Status)PS::Breakout::call(EBOOT(EBOOT_SCE_MSG_DIALOG_UPDATE_STATUS_STUB));
}

int32_t PS::Sce::MsgDialog::Close()
{
    return (int32_t)PS::Breakout::call(DEREF(EBOOT(EBOOT_SCE_MSG_DIALOG_GET_RESULT_STUB_PTR)) + SCE_MSG_DIALOG_CLOSE_OFFSET_GET_RESULT);
}

int32_t PS::Sce::MsgDialog::Terminate()
{
    return (int32_t)PS::Breakout::call(EBOOT(EBOOT_SCE_MSG_DIALOG_TERMINATE_STUB));
}

PS::Sce::MsgDialog::MsgDialog(PS::Sce::MsgDialog::Mode mode) : PS::Sce::CommonDialog::CommonDialog()
{
    if (!PS::Sce::MsgDialog::isInitialized)
    {
        PS::Sce::MsgDialog::Initialize();
        PS::Sce::MsgDialog::isInitialized = true;
    }

    PS2::memset(&this->param, 0x0, sizeof(PS::Sce::MsgDialog::Param));
    this->param.size = sizeof(PS::Sce::MsgDialog::Param);
    this->param.mode = mode;

    PS2::memset(&this->param.baseParam, 0x0, sizeof(PS::Sce::CommonDialog::BaseParam));
    this->param.baseParam.size = (uint32_t)sizeof(PS::Sce::CommonDialog::BaseParam);
    this->param.baseParam.magic = (uint32_t)(SCE_COMMON_DIALOG_MAGIC_NUMBER + VAR_TO_NATIVE(this->param.baseParam));
}

int32_t PS::Sce::MsgDialog::open()
{
    return PS::Sce::MsgDialog::Open(&this->param);
}

int32_t PS::Sce::MsgDialog::getResult(PS::Sce::MsgDialog::Result *result)
{
    return PS::Sce::MsgDialog::GetResult(result);
}

PS::Sce::CommonDialog::Status PS::Sce::MsgDialog::getStatus()
{
    return PS::Sce::MsgDialog::GetStatus();
}

PS::Sce::CommonDialog::Status PS::Sce::MsgDialog::updateStatus()
{
    return PS::Sce::MsgDialog::UpdateStatus();
}

int32_t PS::Sce::MsgDialog::close()
{
    return PS::Sce::MsgDialog::Close();
}

void PS::Sce::MsgDialog::setUserMessageParam(uint64_t param)
{
    this->param.userMsgParam = param;
}

void PS::Sce::MsgDialog::setProgressBarParam(uint64_t param)
{
    this->param.progBarParam = param;
}

void PS::Sce::MsgDialog::setSystemMessageParam(uint64_t param)
{
    this->param.sysMsgParam = param;
}
#endif