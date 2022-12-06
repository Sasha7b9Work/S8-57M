// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"


namespace PainterMem
{
    void FillRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    void SetPoint(int x, int y);

    void SetPoint(int x, int y, Color color);

    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    void DrawRectangle(int x, int y, int widht, int height, Color = Color::NUMBER);
};
