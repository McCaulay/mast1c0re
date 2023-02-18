#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/sce/dialog/msg.hpp>

namespace PS
{
    namespace Sce
    {
        class MsgDialogUserMessage: public MsgDialog
        {
        public:
            typedef struct
            {
                PS::Sce::MsgDialog::ButtonType buttonType;
                int :32;
                uint64_t msg;          // char*
                uint64_t buttonsParam; // PS::Sce::MsgDialog::ButtonsParam*
                char reserved[24];
            } Param;
        public:
            static bool show(const char* message, PS::Sce::MsgDialog::ButtonType type = PS::Sce::MsgDialog::ButtonType::OK);
        public:
            MsgDialogUserMessage(const char* message, PS::Sce::MsgDialog::ButtonType type = PS::Sce::MsgDialog::ButtonType::OK);
        private:
            PS::Sce::MsgDialogUserMessage::Param param;
        };
    }
}
#endif