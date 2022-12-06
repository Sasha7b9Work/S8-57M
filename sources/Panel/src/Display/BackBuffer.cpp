// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/BackBuffer.h"
#include <stdlib.h>


// ¬озвращает адрес байта с координатами x, y.
#define ADDRESS_BYTE(x, y) (buffer + ((y) * width + (x)))

#define SET_POINT(x, y)                             \
    uint8 *address = ADDRESS_BYTE(x, y);            \
    if (address >= buffer && address < endBuffer)   \
    {                                               \
        *address = color.value;                     \
    }


namespace BackBuffer
{
    static const int SIZE_BUFFER = 640 * 480 / 5;

    static uint8 buffer[SIZE_BUFFER] __attribute__((section("CCM_DATA")));

    static uint8 *endBuffer = 0;

    static int width = 0;

    static Color color = Color::FILL;
}


void BackBuffer::SetPoint(int x, int y, Color col)
{
    color = col;

    uint8 *address = ADDRESS_BYTE(x, y);

    if (address >= buffer && address < endBuffer)
    {
        *address = color.value;
    }
}


void BackBuffer::SetPoint(int x, int y)
{
    uint8 *address = ADDRESS_BYTE(x, y);

    if(address >= buffer && address < endBuffer)
    {
        *address = color.value;
    }
}


void BackBuffer::FillRect(int x, int y, int w, int h, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for(int i = 0; i <= h; i++)
    {
        DrawHLine(y + i, x, x + w);
    }
}


void BackBuffer::DrawVLine(int x, int y0, int y1, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for (int y = y0; y <= y1; y++)
    {
        SET_POINT(x, y);
    }
}


void BackBuffer::DrawHLine(int y, int x0, int x1, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for(int x = x0; x <= x1; x++)
    {
        SET_POINT(x, y);
    }
}


void BackBuffer::DrawRectangle(int x, int y, int w, int h, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    DrawVLine(x, y, y + h);
    DrawVLine(x + w, y, y + h);
    DrawHLine(y, x, x + w);
    DrawHLine(y + h, x, x + w);
}
