#include "defines.h"
#include "common/Command.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include <cstring>


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_FillRegion,
        (uint8)(x),
        (uint8)(x >> 8),
        (uint8)(y),
        (uint8)(width),
        (uint8)(width >> 8),
        (uint8)(height)
    };

    HAL_BUS::Panel::Send(buffer, 7);
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_DrawRectangle,
        (uint8)(x),
        (uint8)(x >> 8),
        (uint8)(y),
        (uint8)(width),
        (uint8)(width >> 8),
        (uint8)(height)
    };

    HAL_BUS::Panel::Send(buffer, 7);
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int x0 = x;
    int x1 = x0 + width;

    uint8 buffer[6] =
    {
        Command::Paint_DrawHLine,
        (uint8)(y),
        (uint8)(x0),
        (uint8)(x0 >> 8),
        (uint8)(x1),
        (uint8)(x1 >> 8)
    };

    HAL_BUS::Panel::Send(buffer, 6);
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int y0 = y;
    int y1 = y0 + height;

    uint8 buffer[5] =
    {
        Command::Paint_DrawVLine,
        (uint8)(x),
        (uint8)(x >> 8),
        (uint8)(y0),
        (uint8)(y1)
    };

    HAL_BUS::Panel::Send(buffer, 5);
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[4] =
    {
        Command::Paint_SetPoint,
        (uint8)(x),
        (uint8)(x >> 8),
        (uint8)(y)
    };

    HAL_BUS::Panel::Send(buffer, 4);
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_DrawLine,
        (uint8)(x0),
        (uint8)(x0 >> 8),
        (uint8)(y0),
        (uint8)(x1),
        (uint8)(x1 >> 8),
        (uint8)(y1)
    };

    HAL_BUS::Panel::Send(buffer, 7);
}


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    int sizeBuffer = 1 + 2 + 1 + 1 + (int)std::strlen(text);

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = (uint8)(x);
    buffer.data[2] = (uint8)(x >> 8);
    buffer.data[3] = (uint8)(y);
    buffer.data[4] = (uint8)(std::strlen(text));

    std::memcpy(&buffer.data[5], text, std::strlen(text)); //-V575

    HAL_BUS::Panel::Send(buffer.data, sizeBuffer);

    return x + DFont::GetLengthText(text) + 1;
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
        (uint8)(delta),
        (uint8)(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        buffer[1] = (uint8)(x);
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = y[i];

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
        (uint8)(delta),
        (uint8)(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        int x = x0[i];

        buffer[1] = (uint8)(x);
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = (uint8)(y0);

        HAL_BUS::Panel::Send(buffer, 6);
    }
}
