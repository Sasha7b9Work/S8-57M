// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Text
{
    int Draw(int x, int y, const char *text, int size = 1);

    int DrawChar(int x, int y, int size, uint8 symbol, Color color = Color::Count);

    // ”становка моноширинного режима вывода текста
    void SetMinWidthFont(uint8 width);

    void SetSpacing(uint8 space);
};
