#include "defines.h"
#include "common/Command.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
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

    int x0 = x;
    int x1 = x0 + width;

    SBuffer(Command::Paint_DrawHLine, Point2(x0, y), Point2(x1, 0)).Send();
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int y0 = y;
    int y1 = y0 + height;

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


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    int sizeBuffer = 1 + 2 + 1 + 1 + (int)std::strlen(text);

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = 0;
    buffer.data[2] = 0;
    buffer.data[3] = 0;
    buffer.data[4] = (uint8)(std::strlen(text));

    Point2(x, y).Write(&buffer.data[1]);

    std::memcpy(&buffer.data[5], text, std::strlen(text)); //-V575

    HAL_BUS::Panel::SendBuffer(buffer.data, sizeBuffer);

    return x + Font::GetLengthText(text) + 1;
}
