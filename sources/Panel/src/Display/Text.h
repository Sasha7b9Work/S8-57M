// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


class Text
{
public:
    static int Draw(int x, int y, const char *text);

    static int DrawFormatText(int x, int y, char *format, ...);

    // Пишет строку в позиции x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);

    static int DrawChar(int x, int y, uint8 symbol, Color color = Color::NUMBER);
    // Установка моноширинного режима вывода текста
    static void SetMinWidthFont(uint8 width);

    static void SetSpacing(uint8 space);
};
