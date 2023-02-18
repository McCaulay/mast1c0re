#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/sce/dialog/msg.hpp>
typedef int32_t SceMsgDialogProgressBarTarget;

namespace PS
{
    namespace Sce
    {
        class MsgDialogProgressBar: public MsgDialog
        {
        public:
            typedef enum
            {
                PERCENTAGE = 0,
                PERCENTAGE_CANCEL = 1,
            } Type;

            typedef struct
            {
                PS::Sce::MsgDialogProgressBar::Type barType;
                int :32;
                uint64_t msg; // char*
                char reserved[64];
            } Param;
        public:
            static int32_t Inc(SceMsgDialogProgressBarTarget target, uint32_t delta);
            static int32_t SetMsg(SceMsgDialogProgressBarTarget target, const char *barMsg);
            static int32_t SetValue(SceMsgDialogProgressBarTarget target, uint32_t rate);
        public:
            MsgDialogProgressBar(const char* message, PS::Sce::MsgDialogProgressBar::Type type = PS::Sce::MsgDialogProgressBar::Type::PERCENTAGE);
            int32_t inc(uint32_t delta);
            int32_t setMsg(const char *format, ...);
            int32_t setValue(uint32_t rate);
        private:
            PS::Sce::MsgDialogProgressBar::Param param;
        };
    }
}
#endif