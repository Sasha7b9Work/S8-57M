// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/BackBuffer.h"
#include "Display/Display.h"
#include "Display/Text/Text.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "common/Command.h"
#include "Log.h"
#include <cstdlib>
#include <cstring>
#include <cmath>


namespace BackBuffer
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480 / 5;

    static int field = 0;

    static const int SIZE_BUFFER = WIDTH * HEIGHT;

    static uint8 buffer[SIZE_BUFFER] __attribute__((section("CCM_DATA")));

    static void Fill();

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


    namespace Signal
    {
        static const uint8 col_ch_half[2] = { 254, 253 };   // Индекс половинного цвета
        static const uint8 col_ch_quart[2] = { 252, 251 };  // Индекс четвертного цвета

        // Этими цветами рисуем
        static uint8 col_ch = 0;
        static uint8 col_half = 0;
        static uint8 col_quart = 0;

        // Нарисовать блеклую линию
        static void DrawFadedLine(int x, int y_min, int y_max);
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


void BackBuffer::SetPixel(int x, int y)
{
    uint8 *address = Address::Pixel(x, y);

    if (address >= buffer && address < Address::end)
    {
        *address = Color::current.value;
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
    if (y0 > y1)
    {
        Math::Swap(&y0, &y1);
    }

    int y = y0;

    uint8 color = Color::current.value;

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

    if (x0 > x1)
    {
        Math::Swap(&x0, &x1);
    }

    uint8 *pixel = Address::Pixel(x0, y);

    uint counter = (uint)(x1 - x0 + 1);

    std::memset(pixel, Color::current.value, counter);
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
    std::memset(buffer, Color::current.value, SIZE_BUFFER);
}


void BackBuffer::DrawHPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        SetPixel(x, y);
        x += delta;
    }
}


void BackBuffer::DrawVPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        SetPixel(x, y);
        y += delta;
    }
}


void BackBuffer::DrawVCursor(int x, int y, int height, int /*skip*/)
{
    DrawVLine(x, y, y + height);
}


void BackBuffer::DrawHCursor(int x, int y, int width, int /*skip*/)
{
    DrawHLine(x, y, x + width);
}


void BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        SetPixel(x1, y1);
        return;
    }
    else if (x1 == x2)
    {
        DrawVLine(x1, y1, y2);
        return;
    }
    else if (y1 == y2)
    {
        DrawHLine(y1, x1, x2);
        return;
    }

    int x = x1;
    int y = y1;
    int dx = (int)(std::fabsf((float)(x2 - x1)));
    int dy = (int)(std::fabsf((float)(y2 - y1)));
    int s1 = Math::Sign(x2 - x1);
    int s2 = Math::Sign(y2 - y1);
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        SetPixel(x, y);
        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}


void BackBuffer::LoadPalette()
{
    HAL_LTDC::SetColors(&COLOR(0), 256);
}


void BackBuffer::SendRow(int row)
{
    uint8 *points = Display::GetBuffer() + row * Display::WIDTH;

    uint8 data[Display::WIDTH + 2] = { Command::Screen, (uint8)(row) };

    std::memcpy(&data[2], points, Display::WIDTH);

    HAL_BUS::SendToDevice(data, Display::WIDTH + 2);
}


int BackBuffer::Signal::GetColorIndex(int ch, int type)
{
    if (type == 0)
    {
        return col_ch_half[ch];
    }

    return col_ch_quart[ch];
}


void BackBuffer::Signal::SetChannel(int ch)
{
    col_ch = (ch == 0) ? 3U : 4U;

    col_half = col_ch_half[ch];
    col_quart = col_ch_quart[ch];
}


#define WRITE_BYTE(_addr, value)                    \
    if (_addr >= buffer && _addr < Address::end)    \
    {                                               \
        *_addr = value;                             \
    }

#define WRITE_BYTE_V(_addr, value)                  \
    if (_addr >= buffer && _addr < Address::end)    \
    {                                               \
        if(*_addr != col_ch)                        \
        {                                           \
            *_addr = value;                         \
        }                                           \
    }


void BackBuffer::Signal::DrawPoint(int x, int y)
{
    uint8 *address = Address::Pixel(x, y);

    WRITE_BYTE(address, col_ch);

    static const int shift[4] = { -Display::WIDTH, 1, Display::WIDTH, -1 };

    static const int sh[4] =
    {
        -Display::WIDTH - 1, -Display::WIDTH + 1,
        Display::WIDTH - 1, Display::WIDTH + 1
    };

    for (int i = 0; i < 4; i++)
    {
        uint8 *addr = address + shift[i];
        WRITE_BYTE_V(addr, col_half);

        addr = address + sh[i];
        WRITE_BYTE_V(addr, col_quart);
    }
}


void BackBuffer::Signal::DrawVLine(int x, int y_min, int y_max)
{
    int y = y_min;

    uint8 *pixel = Address::Pixel(x, y);

    while (pixel < buffer)
    {
        pixel += WIDTH;
        y++;
    }

    while (y <= y_max && pixel < Address::end)
    {
        *pixel = col_ch;
        pixel += WIDTH;
        y++;
    }

    DrawFadedLine(x - 1, y_min, y_max);

    DrawFadedLine(x + 1, y_min, y_max);
}


void BackBuffer::Signal::DrawFadedLine(int x, int y_min, int y_max)
{
    int y = y_min;

    uint8 *pixel = Address::Pixel(x, y);

    uint8 *addr = pixel - Display::WIDTH;

    WRITE_BYTE_V(addr, col_quart);

    while (pixel < buffer)
    {
        pixel += WIDTH;
        y++;
    }

    while (y <= y_max && pixel < Address::end)
    {
        if (*pixel != col_ch)
        {
            *pixel = col_half;
        }
        pixel += WIDTH;
        y++;
    }

    WRITE_BYTE_V(pixel, col_quart);
}
