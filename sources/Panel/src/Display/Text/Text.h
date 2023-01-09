// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Text
{
    int Draw(int x, int y, pchar text, int size = 1);

    int DrawChar(int x, int y, int size, uint8 symbol, const Color &color = Color(255));

    // ”становка моноширинного режима вывода текста
    void SetMinWidthFont(uint8 width);

    void SetSpacing(uint8 space);
};
