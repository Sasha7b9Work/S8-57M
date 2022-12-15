// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/Font/Font_p.h"


namespace Painter
{
    // Пересылает строку в устройство
    void SendRow(int row);

    // Загружает в дисплей все цвета
    void LoadPalette();
};
