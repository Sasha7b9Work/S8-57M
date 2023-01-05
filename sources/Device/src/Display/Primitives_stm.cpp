// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include "Hardware/Timer.h"
#include <cstring>


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    SBuffer(Command::Paint_FillRegion, Point2(x, y), Point2(width, height)).Send();
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    SBuffer(Command::Paint_DrawRectangle, Point2(x, y), Point2(width, height)).Send();
}


void HLine::Draw(int x, int y, Color color) 
{
    color.SetAsCurrent();

    const int x0 = x;
    const int x1 = x0 + width;

    SBuffer(Command::Paint_DrawHLine, Point2(x0, y), Point2(x1, 0)).Send();
}


void VCursor::Draw(int x, int y)
{
    SBuffer(Command::Paint_DrawVCursor, Point2(x, y), Point2(height, skip)).Send();;
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    const int y0 = y;
    const int y1 = y0 + height;

    SBuffer(Command::Paint_DrawVLine, Point2(x, y0), Point2(0, y1)).Send();
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    SBuffer(Command::Paint_SetPoint, Point2(x, y)).Send();
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();

    SBuffer(Command::Paint_DrawLine, Point2(x0, y0), Point2(x1, y1)).Send();
}


static int trans_bytes = 0;
static uint time = 0;


int Text::TransBytes()
{
    return trans_bytes;
}


uint Text::ElapsedTime()
{
    return time;
}


void Text::BeginScene()
{
    trans_bytes = 0;
    time = 0;
}


int Text::DrawSmall(int x, int y, Color color)
{
    uint start = TIME_TICKS;

    color.SetAsCurrent();

    int sizeBuffer = 1 + 2 + 1 + 1 + (int)std::strlen(text);

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = 0;
    buffer.data[2] = 0;
    buffer.data[3] = 0;
    buffer.data[4] = (uint8)(std::strlen(text));

    Point2(x, y).Write(&buffer.data[1]);

    std::memcpy(&buffer.data[5], text, std::strlen(text));

    HAL_BUS::Panel::SendBuffer(buffer.data, sizeBuffer);

    trans_bytes += sizeBuffer;

    time += TIME_TICKS - start;

    return x + DFont::GetLengthText(text) + 1;
}


void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        SBuffer buffer(Command::Paint_HPointLine, Point2(x, y[i]));
        buffer.Push((uint8)delta);
        buffer.Push((uint8)count);
        buffer.Send();
    }
}


void MultiVPointLine::Draw(int y0, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        SBuffer buffer(Command::Paint_VPointLine, Point2(x0[i], y0));
        buffer.Push((uint8)delta);
        buffer.Push((uint8)count);
        buffer.Send();
    }
}
