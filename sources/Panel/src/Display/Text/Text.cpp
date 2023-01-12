// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text/Text.h"
#include "Display/BackBuffer.h"
#include "common/Display/Font/Font.h"
#include <cstring>



// Если true - текст будет моноширинным
static uint8 minWidth = 0;
// Горизонтальное расстояние между символами при выводе текста
static uint8 spacing = 1;



void Text::SetMinWidthFont(uint8 width)
{
    minWidth = width;
}


void Text::SetSpacing(uint8 space)
{
    spacing = space;
}


int Text::DrawChar(int eX, int eY, uint8 symbol, const Color &color)
{
    color.SetAsCurrent();

    uint8 width = Font::GetWidth(symbol);
    int height = Font::GetHeight();

    int delta = 9 - height;

    for (int row = 0; row < height; row++)
    {
        if (Font::RowNotEmpty(symbol, row))
        {
            int x = eX;
            int y = eY + row + delta;
            for (int bit = 0; bit < width; bit++)
            {
                if (Font::BitIsExist(symbol, row, bit))
                {
                    BackBuffer::SetPixel(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


int Text::Draw(int x, int y, pchar text)
{
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        int prevX = x;
        x = DrawChar(x, y, (uint8)(text[i]));

        if (x - prevX < minWidth)
        {
            x = prevX + minWidth;
        }

        x += spacing;
    }

    return x;
}
