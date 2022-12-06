// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/BackBuffer.h"
#include "Display/Painter.h"
#include <stdlib.h>


namespace BackBuffer
{
    static const int SIZE_BUFFER = 640 * 480 / 5;

    static uint8 buffer[SIZE_BUFFER] __attribute__((section("CCM_DATA")));

    static uint8 *AddressByte(int x, int y);

    static uint8 *endBuffer = &buffer[0] + SIZE_BUFFER;
}


uint8 *BackBuffer::AddressByte(int x, int y)
{
    return nullptr;
}


void BackBuffer::SetPoint(int x, int y)
{
    Color color = Color::Current();

    uint8 *address = AddressByte(x, y);

    if (address >= buffer && address < endBuffer)
    {
        *address = color.value;
    }
}


void BackBuffer::FillRect(int x, int y, int w, int h)
{
    for(int i = 0; i <= h; i++)
    {
        DrawHLine(y + i, x, x + w);
    }
}


void BackBuffer::DrawVLine(int x, int y0, int y1)
{
    for (int y = y0; y <= y1; y++)
    {
        SetPoint(x, y);
    }
}


void BackBuffer::DrawHLine(int y, int x0, int x1)
{
    for(int x = x0; x <= x1; x++)
    {
        SetPoint(x, y);
    }
}


void BackBuffer::DrawRectangle(int x, int y, int w, int h)
{
    DrawVLine(x, y, y + h);
    DrawVLine(x + w, y, y + h);
    DrawHLine(y, x, x + w);
    DrawHLine(y + h, x, x + w);
}
