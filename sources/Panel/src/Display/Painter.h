// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/Font/Font_p.h"


struct Painter
{
    // Пересылает строку в устройство
    static void SendRow(int row);

    // Загружает в дисплей все цвета
    static void LoadPalette();

    static void DrawVPointLine(int x, int y, int delta, int count);

    static void DrawHPointLine(int x, int y, int delta, int count);

    // Нарисовать горизонтальную линию
    static void DrawHLine(int y, int x0, int x1);

    // Нарисовать вертикальную линию
    static void DrawVLine(int x, int y0, int y1);

    // Нарисовать произвольную линию
    static void DrawLine(int x0, int y0, int x1, int y1);

    // \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // Рисует прерывистую вертикальную линию.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static int DrawFormatText(int x, int y, char *format, ...);

    // Пишет строку в позиции x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);

    // Посылает строку изображения numString в устройство
    static void SendScreenToDevice();

private:
    static void FillRegion(int x, int y, int width, int height);

    // Нарисовать точку текущим цветом
    static void SetPoint(int x, int y);
};
