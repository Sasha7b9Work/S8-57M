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


void HLine::Draw(int, int, Color)
{
    // todo_paint
}


void VLine::Draw(int, int, Color)
{
    // todo_paint
}


void Pixel::Draw(int, int, Color)
{
    // todo_paint
}


void Line::Draw(Color)
{
    // todo_paint
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
