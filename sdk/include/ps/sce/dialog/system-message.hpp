#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <types.hpp>
#include <ps/sce/dialog/msg.hpp>

namespace PS
{
    namespace Sce
    {
        class MsgDialogSystemMessage: public MsgDialog
        {
        public:
            typedef enum
            {
                TRC_EMPTY_STORE = 0,
                TRC_PSN_CHAT_RESTRICTION = 1,
                TRC_PSN_UGC_RESTRICTION = 2,
                CAMERA_NOT_CONNECTED = 4,
                WARNING_PROFILE_PICTURE_AND_NAME_NOT_SHARED = 5,
            } Type;

            typedef struct
            {
                PS::Sce::MsgDialogSystemMessage::Type sysMsgType;
                char reserved[32];
            } Param;
        public:
            MsgDialogSystemMessage();
        };
    }
}
#endif