#include "defines.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/LTDC.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <cstring>
#include <cmath>

#include "Painter_common.h"

// ������������� � true �������� ��������, ��� �������� ����� ������� � ����������
static int sendingString = -1;


void Painter::BeginScene()
{   
    Color col = currentColor;

    uint *address = reinterpret_cast<uint *>(Display::GetBuffer());

    uint *end = address + (BUFFER_HEIGHT * BUFFER_WIDTH) / 4; //-V2563

    uint value = static_cast<uint>(col.value) + static_cast<uint>(col.value << 8) + static_cast<uint>(col.value << 16) + static_cast<uint>(col.value << 24);

    while (address != end)
    {
        *address++ = value;
    }
}


void Painter::SetColorValue(Color color, uint value)
{
    COLOR(color.value) = value;

    LoadPalette();
}


void Painter::LoadPalette()
{
    LTDC_::SetColors(&COLOR(0), Color::NUMBER.value);
}


void Painter::EndScene(void)
{
    if (sendingString >= 0)                                               // ���� ����� ��������� ��������
    {
#define SIZE  (SIZE_STRING + 2)

        uint8 buffer[SIZE] = { Command::Screen, static_cast<uint8>(sendingString) };

        std::memcpy(buffer + 2, Display::GetBuffer() + sendingString * SIZE_STRING, SIZE_STRING); //-V2563

        HAL_BUS::SendToDevice(buffer, SIZE_STRING);

        sendingString++;
        if (sendingString == 120)
        {
            sendingString = -1;
        }
    }

    LTDC_::ToggleBuffers();
}


void Painter::DrawHLine(int y, int x0, int x1)
{
    if (x0 < 0) { x0 = 0; }
    if (x1 < 0) { x1 = 0; }
    if (y < 0)  { y = 0;  }
    
    if(x0 > x1)
    {
        Math::Swap(&x0, &x1);
    }

    uint8 *address = Display::GetBuffer() + x0 + y * BUFFER_WIDTH; //-V2563
    uint8 *end = Display::GetBufferEnd();

    uint8 value = currentColor.value;

    for (int x = x0; x <= x1; ++x)
    {
        if (address >= end)
        {
            break;
        }
        *address++ = value;
    }
}


void Painter::DrawLine(int x1, int y1, int x2, int y2) //-V2506
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


void Painter::DrawRectangle(int x, int y, int width, int height)
{
    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}


void Painter::DrawVLine(int x, int y0, int y1)
{
    if(y0 > y1)
    {
        Math::Swap(&y0, &y1);
    }

    uint8 *address = Display::GetBuffer() + x + y0 * BUFFER_WIDTH; //-V2563
    uint8 *end = Display::GetBufferEnd();

    uint8 value = currentColor.value;

    for (int y = y0; y <= y1; ++y)
    {
        if (address > end)
        {
            break;
        }

        *address = value;
        address += BUFFER_WIDTH; //-V2563
    }
}


void Painter::FillRegion(int x, int y, int width, int height)
{
    for (int i = y; i <= y + height; ++i)
    {
        DrawHLine(i, x, x + width);
    }
}


void Painter::SetColor(Color color)
{
    if (color.value != Color::NUMBER.value)
    {
        currentColor = color;
    }
}


void Painter::SetPoint(int x, int y)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT)
    {
        *(Display::GetBuffer() + y * BUFFER_WIDTH + x) = currentColor.value; //-V2563
    }
}


void Painter::SendRow(int row)
{
    uint8 *points = Display::GetBuffer() + row * BUFFER_WIDTH; //-V2563

    uint8 data[322] = { Command::Screen, static_cast<uint8>(row) };

    std::memcpy(&data[2], points, 320);

    HAL_BUS::SendToDevice(data, 322);
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
    sendingString = 0;
}
