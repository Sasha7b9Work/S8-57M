// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/BackBuffer.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include <cstdlib>
#include <cstring>


namespace BackBuffer
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480 / 5;

    static int field = 0;

    static const int SIZE_BUFFER = WIDTH * HEIGHT;

    static uint8 buffer[SIZE_BUFFER] __attribute__((section("CCM_DATA")));

    static uint8 *AddressByte(int x, int y)
    {
        y -= field * HEIGHT;

        return &buffer[x + y * WIDTH];
    }

    static void Fill();

    // Проверяет на попадание в буфер
    namespace Limit
    {
        bool X(int x)
        {
            return x >= 0 && x < WIDTH;
        }

        bool Y(int y)
        {
            return (y >= field * HEIGHT) && y < ((field + 1) *HEIGHT + 1);
        }
    }
}


void BackBuffer::BeginPaint(int _field)
{
    field = _field;

    Fill();
}


void BackBuffer::EndPaint()
{
    uint offset = (uint)(field * WIDTH * HEIGHT);

    Display::SetField(offset, buffer, WIDTH * HEIGHT);
}


void BackBuffer::SetPoint(int x, int y)
{
    if (Limit::X(x) && Limit::Y(y))
    {
        *AddressByte(x, y) = Color::Current().value;
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


void BackBuffer::Fill()
{
    std::memset(buffer, Color::Current().value, SIZE_BUFFER);
}
