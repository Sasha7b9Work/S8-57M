// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/BackBuffer.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Display/Text/Text.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <cstdlib>
#include <cstring>


namespace BackBuffer
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480 / 5;

    static int field = 0;

    static const int SIZE_BUFFER = WIDTH * HEIGHT;

    static uint8 buffer[SIZE_BUFFER] __attribute__((section("CCM_DATA")));

    static void Fill();

    int MinY()
    {
        return field * HEIGHT;
    }

    int MaxY()
    {
        return (field + 1) * HEIGHT;
    }

    namespace Address
    {
        uint8 *end = buffer + SIZE_BUFFER;

        uint8 *Pixel(int x, int y)
        {
            y -= field * HEIGHT;

            return &buffer[x + y * WIDTH];
        }
    }

    // Проверяет на попадание в буфер
    namespace Limit
    {
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
    uint8 *address = Address::Pixel(x, y);

    if (address >= buffer && address < Address::end)
    {
        *address = Color::Current().value;
    }
}


void BackBuffer::FillRegion(int x, int y, int w, int h)
{
    for(int i = 0; i <= h; i++)
    {
        DrawHLine(y + i, x, x + w);
    }
}


void BackBuffer::DrawVLine(int x, int y0, int y1)
{
    // \todo Без проверок работает ощутимо быстрее

//    if (y0 > y1)
//    {
//        Math::Swap(&y0, &y1);
//    }

    int y = y0;

    uint8 color = Color::Current().value;

    uint8 *pixel = Address::Pixel(x, y);

    while (pixel < buffer)
    {
        pixel += WIDTH;
        y++;
    }

    while (y <= y1 && pixel < Address::end)
    {
        *pixel = color;
        pixel += WIDTH;
        y++;
    }
}


void BackBuffer::DrawHLine(int y, int x0, int x1)
{
    if (!Limit::Y(y))
    {
        return;
    }

    // \todo Без проверок работает ощутимо быстрее

//    if (x0 > x1)
//    {
//        Math::Swap(&x0, &x1);
//    }

    uint8 *pixel = Address::Pixel(x0, y);

    uint counter = (uint)(x1 - x0 + 1);

    std::memset(pixel, Color::Current().value, counter);
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


void BackBuffer::DrawHPointLine(int x, int y, int delta, int count)
{

}


void BackBuffer::DrawVPointLine(int x, int y, int delta, int count)
{

}


void BackBuffer::DrawLine(int x0, int y0, int x1, int y1)
{

}
