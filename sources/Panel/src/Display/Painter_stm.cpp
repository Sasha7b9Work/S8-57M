// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/AveragerTester.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Display/MemPainter.h"
#include <cstring>
#include <cmath>

#include "Painter_common.h"


void Painter::SetColorValue(Color color, uint value)
{
    COLOR(color.value) = value;

    LoadPalette();
}


void Painter::LoadPalette()
{
    HAL_LTDC::SetColors(&COLOR(0), Color::NUMBER.value);
}


void Painter::EndScene(void)
{
    // todo_paint
}


void Painter::DrawLine(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        MemPainter::SetPoint(x1, y1);
        return;
    }
    else if (x1 == x2)
    {
        MemPainter::DrawVLine(x1, y1, y2);
        return;
    }
    else if (y1 == y2) //-V2516
    {
        MemPainter::DrawHLine(y1, x1, x2);
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
        MemPainter::SetPoint(x, y);
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


void Painter::DrawRectangle(int x, int y, int width, int height)
{
    MemPainter::DrawHLine(y, x, x + width);
    MemPainter::DrawHLine(y + height, x, x + width);
    MemPainter::DrawVLine(x, y, y + height);
    MemPainter::DrawVLine(x + width, y, y + height);
}


void Painter::FillRegion(int x, int y, int width, int height)
{
    for (int i = y; i <= y + height; ++i)
    {
        MemPainter::DrawHLine(i, x, x + width);
    }
}


void Painter::SetColor(Color color)
{
    if (color.value != Color::NUMBER.value)
    {
        currentColor = color;
    }
}


void Painter::DrawTesterData(uint8 mode, Color color, const uint16 _x[TESTER_NUM_POINTS], const uint8 _y[TESTER_NUM_POINTS]) //-V2009
{
    SetColor(color);

    int step = EXTRACT_STEP(mode);

    int numAverage = EXTRACT_ENUM_AVERAGE(mode);
   
    AveragerTester::SetCount(numAverage);
    AveragerTester::Process(_x, _y, step);

    uint16 *x = AveragerTester::X();
    uint8 *y = AveragerTester::Y();
    
    if(EXTRACT_MODE_DRAW(mode))
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            FillRegion(x[i], y[i], 2, 2);
            //*(Display::GetBuffer() + y[i] * BUFFER_WIDTH + x[i]) = currentColor.value;
        }
    }
    else
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            DrawLine(x[i], y[i], x[i + 1], y[i + 1]);
        }
    }
}


void Painter::SendRow(int)
{
    // todo_paint
}


uint Painter::ReduceBrightness(uint colorValue, float newBrightness)
{
    int red = static_cast<int>(static_cast<float>(R_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(red, 0, 0xff); //-V2516
    int green = static_cast<int>(static_cast<float>(G_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(green, 0, 0xff); //-V2516
    int blue = static_cast<int>(static_cast<float>(B_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(blue, 0, 0xff); //-V2516
    return MAKE_COLOR(red, green, blue);
}


void Painter::SendScreenToDevice()
{
}
