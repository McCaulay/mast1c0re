#include <mast1c0re.hpp>

void main()
{
    // PS2 Breakout
    PS::Breakout::init();

    #if (defined(PS4) && PS4)
    bool isPS5 = false;
    #elif (defined(PS5) && PS5)
    bool isPS5 = true;
    #endif

    // Show notification YES/NO, then OK
    if (PS::Sce::MsgDialogUserMessage::show("Are you on a PS5?", PS::Sce::MsgDialog::ButtonType::YESNO) == isPS5)
        PS::Sce::MsgDialogUserMessage::show("Wow, you are so honest!");
    else
        PS::Sce::MsgDialogUserMessage::show("LIAR!");

    // Restore corruption
    PS::Breakout::restore();
}