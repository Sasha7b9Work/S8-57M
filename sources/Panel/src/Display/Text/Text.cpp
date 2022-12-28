// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text/Text.h"
#include "Display/BackBuffer.h"
#include "Display/Font/Font_p.h"
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


int Text::DrawChar(int eX, int eY, int size, uint8 symbol, Color color)
{
    color.SetAsCurrent();

    uint8 width = PFont::GetWidth(symbol);
    uint8 height = PFont::GetHeight();

    int delta = PFont::IsBig() ? 0 : (9 - height);

    if (size == 1)
    {
        for (int row = 0; row < height; row++)
        {
            if (PFont::RowNotEmpty(symbol, row))
            {
                int x = eX;
                int y = eY + row + delta;
                for (int bit = 0; bit < width; bit++)
                {
                    if (PFont::BitIsExist(symbol, row, bit))
                    {
                        BackBuffer::SetPoint(x, y);
                    }
                    x++;
                }
            }
        }
    }
    else
    {
        for (int row = 0; row < height; row++)
        {
            if (PFont::RowNotEmpty(symbol, row))
            {
                int x = eX;
                int y = eY + (row + delta) * size;
                for (int bit = 0; bit < width; bit++)
                {
                    if (PFont::BitIsExist(symbol, row, bit))
                    {
                        //                        Painter::SetPoint(x, y);
                        BackBuffer::FillRegion(x, y, size - 1, size - 1);
                    }
                    x += size;
                }
            }
        }
    }

    return eX + width * size;
}


int Text::Draw(int x, int y, pchar text, int size)
{
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        int prevX = x;
        x = DrawChar(x, y, size, (uint8)(text[i]));

        if (x - prevX < minWidth)
        {
            x = prevX + minWidth;
        }

        x += spacing * size;
    }

    return x;
}
