// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


typedef void (*pFuncVChI16)(Ch::E, int16);
typedef void (*pFuncVCh)(Ch::E);
typedef void (*pFuncVI)(int);


namespace Handlers
{
    void Process(KeyEvent);
};
