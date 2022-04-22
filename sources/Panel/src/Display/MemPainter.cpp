// 2022/04/22 11:40:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/MemPainter.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Font/Font_p.h"
#include "Display/Painter_common.h"
#include <cstring>
#include <cmath>


#define CALCULATE_X()  x -= x_shift; if (x < 0) { return; } if (x >= width)  { return; }
#define CALCULATE_Y()  y -= y_shift; if (y < 0) { return; } if (y >= height) { return; }


namespace MemPainter
{
    uint8 buffer[64 * 1024] __attribute__((section("CCM_DATA")));;

    int x_shift = 0;
    int y_shift = 0;
    int width = 0;
    int height = 0;
}


void MemPainter::BeginPaint(int x, int y, int w, int h)
{
    x_shift = Math::Limitation(x, 0, Display::WIDTH - 1);
    y_shift = Math::Limitation(y, 0, Display::HEIGHT - 1);

    width = Math::Limitation(w, 0, 640);
    height = Math::Limitation(h, 0, 480);

    if (width + x_shift >= Display::WIDTH)
    {
        width = Display::WIDTH - x_shift;
    }

    if (height + y_shift >= Display::HEIGHT)
    {
        height = Display::HEIGHT - y_shift;
    }
}


void MemPainter::SetPoint(int x, int y)
{
    CALCULATE_X();
    CALCULATE_Y();

    *(buffer + y * Display::WIDTH * y + x) = currentColor.value;
}


void MemPainter::Fill()
{
    std::memset(buffer, currentColor.value, (uint)(width * height));
}


void MemPainter::EndPaint()
{
    HAL_LTDC::CopyImage(buffer, x_shift, y_shift, width, height);
}


void MemPainter::DrawHLine(int y, int x1, int x2)
{
    if (y < 0 || y >= height) { return; }
    if (x1 < 0 && x2 < 0) { return; }
    if (x1 >= width && x2 >= width) { return; }
    if (x1 < 0) { x1 = 0; }
    if (x2 < 0) { x2 = 0; }
    if (x1 >= width) { x1 = width - 1; }
    if (x2 >= width) { x2 = width - 1; }

    if (x1 > x2)
    {
        Math::Swap(&x1, &x2);
    }

    std::memset(buffer + y * width + x1, currentColor.value, (uint)(x2 - x1));
}


void MemPainter::DrawVLine(int x, int y1, int y2)
{
    if (x < 0 || x >= width) { return; }
    if (y1 < 0 && y2 < 0) { return; }
    if (y1 >= width && y2 >= width) { return; }
    if (y1 < 0) { y1 = 0; }
    if (y2 < 0) { y2 = 0; }
    if (y1 >= height) { y1 = height - 1; }
    if (y2 >= height) { y2 = height - 1; }

    if (y1 > y2)
    {
        Math::Swap(&y1, &y2);
    }

    uint8 *address = buffer + y1 * width + x;

    uint8 color = currentColor.value;

    int length = y2 - y1;

    while (length-- > 0)
    {
        *address = color;

        address += width;
    }
}


void MemPainter::DrawVPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        SetPoint(x, y);
        y += delta;
    }
}


void MemPainter::DrawHPointLine(int x, int y, int delta, int count)
{
    for (int i = 0; i < count; i++)
    {
        SetPoint(x, y);
        x += delta;
    }
}


void MemPainter::DrawRectangle(int x, int y, int w, int h)
{
    DrawHLine(y, x, x + w);
    DrawHLine(y + h, x, x + w);
    DrawVLine(x, y, y + h);
    DrawVLine(x + w, y, y + h);
}


void MemPainter::FillRegion(int x, int y, int w, int h)
{
    for (int i = y; i <= y + h; ++i)
    {
        DrawHLine(i, x, x + w);
    }
}


void MemPainter::DrawLine(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        SetPoint(x1, y1);
        return;
    }
    else if (x1 == x2)
    {
        DrawVLine(x1, y1, y2);
        return;
    }
    else if (y1 == y2) //-V2516
    {
        DrawHLine(y1, x1, x2);
        return;
    }

    int x = x1;
    int y = y1;
    int dx = static_cast<int>(std::fabsf(static_cast<float>(x2 - x1)));
    int dy = static_cast<int>(std::fabsf(static_cast<float>(y2 - y1)));
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
        SetPoint(x, y);

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


void MemPainter::DrawDashedVLine(int x, int y0, int y2, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        return;
    }

    int y = y0;

    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y2)
    {
        DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}
