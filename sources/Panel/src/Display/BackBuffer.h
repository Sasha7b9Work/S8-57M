﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"


namespace BackBuffer
{
    // Очищает часть экрана field цветов color.
    // field может принимать значения [0...4]. Экран разделён на пять зон размерами 640 * 96 пикселей
    void BeginPaint(int field);

    void EndPaint();

    void FillRegion(int x, int y, int width, int height);

    void SetPoint(int x, int y);

    void DrawVLine(int x, int y0, int y1);

    void DrawHLine(int y, int x0, int x1);

    void DrawRectangle(int x, int y, int widht, int height);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);

    void DrawDashedVLine(int x, int y, int height, int deltaFill, int deltaEmpty, int deltaStart);

    void DrawLine(int x0, int y0, int x1, int y1);

    // Пересылает строку в устройство
    void SendRow(int row);

    // Загружает в дисплей все цвета
    void LoadPalette();
};
