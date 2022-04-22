// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Painter.h"
#include "Display/Colors.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Painter_common.h"
#include "Display/MemPainter.h"


void Painter::DrawVPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        MemPainter::SetPoint(x, y);
        y += delta;
    }
}


void Painter::DrawHPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        MemPainter::SetPoint(x, y);
        x += delta;
    }
}


void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
//        LOG_ERROR_TRACE("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            MemPainter::DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        MemPainter::DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
//        LOG_ERROR_TRACE("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            MemPainter::DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        MemPainter::DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}


Color Painter::GetColor()
{
    return currentColor;
}
