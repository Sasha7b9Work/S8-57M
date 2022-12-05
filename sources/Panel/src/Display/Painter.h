// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font_p.h"


namespace Painter
{
    // Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    void EndScene();

    // Пересылает строку в панель
    void SendRow(int row);

    void SetColor(Color color);

    // Возвращает текущий цвет рисования
    Color GetColor();

    // Устанавливает цвету заданное значение. Загружается в дисплей LoadPalette() или SetPalette()
    void SetColorValue(Color color, uint value);

    // Загружает в дисплей все цвета
    void LoadPalette();

    // Установить цвет рисования
    // Нарисовать точку текущим цветом
    void SetPoint(int x, int y);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);

    // Нарисовать горизонтальную линию
    void DrawHLine(int y, int x0, int x1);

    // Нарисовать вертикальную линию
    void DrawVLine(int x, int y0, int y1);

    // Нарисовать произвольную линию
    void DrawLine(int x0, int y0, int x1, int y1);

    // \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // Рисует прерывистую вертикальную линию.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height);

    void FillRegion(int x, int y, int width, int height);

    uint ReduceBrightness(uint colorValue, float newBrightness);

    int DrawFormatText(int x, int y, char *format, ...);

    // Пишет строку в позиции x, y
    int DrawFormText(int x, int y, Color color, pString text, ...);

    // Посылает строку изображения numString в устройство
    void SendScreenToDevice();
};
