#pragma once
#include "Display/Colors.h"


namespace Text
{
    int Draw(int x, int y, const char *text);

    int DrawChar(int x, int y, uint8 symbol, Color color = Color::NUMBER);

    // ”становка моноширинного режима вывода текста
    void SetMinWidthFont(uint8 width);

    void SetSpacing(uint8 space);
};
