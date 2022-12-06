﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"


namespace BackBuffer
{
    // Очищает часть экрана field цветов color.
    // field может принимать значения [0...4]. Экран разделён на пять зон размерами 640 * 96 пикселей
    void BeginPaint(int field, Color color);

    void FillRect(int x, int y, int width, int height, Color color = Color::Count);

    void SetPoint(int x, int y);

    void SetPoint(int x, int y, Color color);

    void DrawVLine(int x, int y0, int y1, Color color = Color::Count);

    void DrawHLine(int y, int x0, int x1, Color color = Color::Count);

    void DrawRectangle(int x, int y, int widht, int height, Color = Color::Count);
};
