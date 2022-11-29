// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls.h"
#include "defines.h"


namespace Keyboard
{
    void Init();

    void Update();

    //  оличество выводов RL дл€ опроса клавиатуры
    const int NUM_RL = 6;

    //  оличество выводов SL дл€ опроса клавиатуры
    const int NUM_SL = 8;

    // ¬озращает им€ органа управлени€
    const char *ControlName(Control control);
};
