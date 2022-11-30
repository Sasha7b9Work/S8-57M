// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"


void EmptyFuncVV()
{

}


void EmptyFuncVB(bool)
{

}


volatile BitField gBF =
{
    0,  // currentNumROMSignal
    0,  // showHelpHints
    1,  // showDebugMenu
    0,  // needForSaveToFlashDrive
    0,  // exitFromModeSetNameTo
    0,  // temporaryShowStrNavi
    0,  // runningFPGAbeforeDrawButtons
    0   // exitFromROMtoRAM
};
