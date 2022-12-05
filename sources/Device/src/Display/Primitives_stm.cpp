// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include <cstring>


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] = { Command::Paint_FillRegion };

    Point2(x, y).Write(&buffer[1]);
    Point2(width, height).Write(&buffer[4]);

    HAL_BUS::Panel::Send(buffer, 7);
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] = { Command::Paint_DrawRectangle };

    Point2(x, y).Write(&buffer[1]);
    Point2(width, height).Write(&buffer[4]);

    HAL_BUS::Panel::Send(buffer, 7);
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] = { Command::Paint_DrawHLine };

    const int x0 = x;
    const int x1 = x0 + width;

    Point2(x0, y).Write(&buffer[1]);
    Point2(x1, 0).Write(&buffer[4]);

    HAL_BUS::Panel::Send(buffer, 6);
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] = { Command::Paint_DrawVLine };

    const int y0 = y;
    const int y1 = y0 + height;

    Point2(x, y0).Write(&buffer[1]);
    Point2(0, y1).Write(&buffer[4]);

    HAL_BUS::Panel::Send(buffer, 5);
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[4] = { Command::Paint_SetPoint };

    Point2(x, y).Write(&buffer[1]);

    HAL_BUS::Panel::Send(buffer, 4);
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] = { Command::Paint_DrawLine };

    Point2(x0, y0).Write(&buffer[1]);
    Point2(x1, y1).Write(&buffer[4]);

    HAL_BUS::Panel::Send(buffer, 7);
}


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    int sizeBuffer = 1 + 2 + 1 + 1 + static_cast<int>(std::strlen(text)); //-V2513

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = 0;
    buffer.data[2] = 0;
    buffer.data[3] = 0;
    buffer.data[4] = static_cast<uint8>(std::strlen(text)); //-V1029 //-V2513

    Point2(x, y).Write(&buffer.data[1]);

    std::memcpy(&buffer.data[5], static_cast<void *>(const_cast<char *>(text)), std::strlen(text)); //-V2513 //-V2567

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
        static_cast<uint8>(delta),
        static_cast<uint8>(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        Point2(x, y[i]).Write(&buffer[1]);

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
        Point2(x0[i], y0).Write(&buffer[1]);

        HAL_BUS::Panel::Send(buffer, 6);
    }
}
