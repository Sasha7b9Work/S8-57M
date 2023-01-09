// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"


namespace BackBuffer
{
    // Очищает часть экрана field цветов color.
    // field может принимать значения [0...4]. Экран разделён на пять зон размерами 640 * 96 пикселей
    void BeginPaint(int field);

    void EndPaint();

    void FillRegion(int x, int y, int width, int height);

    void SetPixel(int x, int y);

    void DrawVLine(int x, int y0, int y1);

    void DrawHLine(int y, int x0, int x1);

    void DrawRectangle(int x, int y, int widht, int height);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);

    void DrawVCursor(int x, int y, int height, int skip);

    void DrawHCursor(int x, int y, int width, int skip);

    void DrawLine(int x0, int y0, int x1, int y1);

    // Пересылает строку в устройство
    void SendRow(int row);

    // Загружает в дисплей все цвета
    void LoadPalette();

    namespace Signal
    {
        void Point(int x, int y);
    }
};
