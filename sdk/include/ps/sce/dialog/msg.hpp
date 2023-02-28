#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/sce/dialog/common.hpp>

#define SCE_MSG_DIALOG_BUTTON_ID_INVALID 0
#define SCE_MSG_DIALOG_BUTTON_ID_OK      1
#define SCE_MSG_DIALOG_BUTTON_ID_YES     1
#define SCE_MSG_DIALOG_BUTTON_ID_NO      2
#define SCE_MSG_DIALOG_BUTTON_ID_BUTTON1 1
#define SCE_MSG_DIALOG_BUTTON_ID_BUTTON2 2

typedef int32_t SceUserServiceUserId;
typedef int32_t SceMsgDialogButtonId;

namespace PS
{
    namespace Sce
    {
        class MsgDialog: public CommonDialog
        {
        public:
            typedef enum
            {
                USER_MSG = 1,
                PROGRESS_BAR = 2,
                SYSTEM_MSG = 3,
            } Mode;

            typedef struct
            {
                PS::Sce::MsgDialog::Mode mode;
                int32_t result;
                SceMsgDialogButtonId buttonId;
                char reserved[32];
            } Result;

            typedef enum
            {
                INVALID = 0,
                OK_YES = 1,
                NO_CANCEL = 2,
            } ButtonId;

            typedef enum
            {
                OK = 0,
                YESNO = 1,
                NONE = 2,
                OK_CANCEL = 3,
                WAIT = 5,
                WAIT_CANCEL = 6,
                YESNO_FOCUS_NO = 7,
                OK_CANCEL_FOCUS_CANCEL = 8,
                TWO_BUTTONS = 9,
            } ButtonType;

            typedef struct
            {
                uint64_t msg1; // char*
                uint64_t msg2; // char*
                char reserved[32];
            } ButtonsParam;

            typedef struct
            {
                PS::Sce::CommonDialog::BaseParam baseParam;
                size_t size;
                PS::Sce::MsgDialog::Mode mode;
                int :32;
                uint64_t userMsgParam; // PS::Sce::MsgDialogUserMessage::Param*
                uint64_t progBarParam; // PS::Sce::MsgDialogProgressBar::Param*
                uint64_t sysMsgParam;  // PS::Sce::MsgDialogSystemMessage::Param*
                SceUserServiceUserId userId;
                char reserved[40];
                int :32;
            } Param;
        public:
            static int32_t Initialize();
            static int32_t Open(PS::Sce::MsgDialog::Param *param);
            static int32_t GetResult(PS::Sce::MsgDialog::Result *result);
            static PS::Sce::CommonDialog::Status GetStatus();
            static PS::Sce::CommonDialog::Status UpdateStatus();
            static int32_t Close();
            static int32_t Terminate();
        private:
            static bool isInitialized;
        public:
            MsgDialog(PS::Sce::MsgDialog::Mode mode);
            int32_t open();
            int32_t getResult(PS::Sce::MsgDialog::Result *result);
            PS::Sce::CommonDialog::Status getStatus();
            PS::Sce::CommonDialog::Status updateStatus();
            int32_t close();
        protected:
            void setUserMessageParam(uint64_t param);
            void setProgressBarParam(uint64_t param);
            void setSystemMessageParam(uint64_t param);
        private:
            PS::Sce::MsgDialog::Param param;
        };
    }
}
#endif