#include "defines.h"
#include "common/Command.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include <cstring>


void Region::Fill(int, int, Color)
{
    // todo_paint
}


void Rectangle::Draw(int, int, Color)
{
    // todo_paint
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int x0 = x;
    int x1 = x0 + width;

    uint8 buffer[6] =
    {
        Command::Paint_DrawHLine,
        static_cast<uint8>(y),
        static_cast<uint8>(x0),
        static_cast<uint8>(x0 >> 8),
        static_cast<uint8>(x1),
        static_cast<uint8>(x1 >> 8)
    };

    HAL_BUS::Panel::Send(buffer, 6);
}


void VLine::Draw(int, int, Color)
{
    // todo_paint
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[4] =
    {
        Command::Paint_SetPoint,
        static_cast<uint8>(x),
        static_cast<uint8>(x >> 8),
        static_cast<uint8>(y)
    };

    HAL_BUS::Panel::Send(buffer, 4);
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_DrawLine,
        static_cast<uint8>(x0),
        static_cast<uint8>(x0 >> 8),
        static_cast<uint8>(y0),
        static_cast<uint8>(x1),
        static_cast<uint8>(x1 >> 8),
        static_cast<uint8>(y1)
    };

    HAL_BUS::Panel::Send(buffer, 7);
}


int Text::DrawSmall(int, int, Color)
{
    // todo_paint

    return 0;
}


void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] =
    {
        Command::Paint_HPointLine,
        0,
        0,
        0,
        static_cast<uint8>(delta),
        static_cast<uint8>(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        buffer[1] = static_cast<uint8>(x);
        buffer[2] = static_cast<uint8>(x >> 8);
        buffer[3] = y[i]; //-V2563

        HAL_BUS::Panel::Send(buffer, 6);
    }
}


void MultiVPointLine::Draw(int y0, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] =
    {
        Command::Paint_VPointLine,
        0,
        0,
        0,
        static_cast<uint8>(delta),
        static_cast<uint8>(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        int x = x0[i]; //-V2563

        buffer[1] = static_cast<uint8>(x);
        buffer[2] = static_cast<uint8>(x >> 8);
        buffer[3] = static_cast<uint8>(y0);

        HAL_BUS::Panel::Send(buffer, 6);
    }
}
