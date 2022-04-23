// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text.h"
#include "Display/MemPainter.h"
#include "Display/Font/Font_p.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>


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


int Text::DrawChar(int eX, int eY, uint8 symbol, Color color)
{
    color.SetAsCurrent();
    
    uint8 width = PFont::GetWidth(symbol);
    uint8 height = PFont::GetHeight();

    int delta = PFont::IsBig() ? 0 : (9 - height);

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
                    MemPainter::SetPoint(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


int Text::Draw(int x, int y, const char *text)
{
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        int prevX = x;
        x = DrawChar(x, y, static_cast<uint8>(text[i]));

        if (x - prevX < minWidth)
        {
            x = prevX + minWidth;
        }

        x += spacing;
    }

    return x;
}


int Text::DrawFormatText(int x, int y, char *format, ...)
{
    char buffer[200];
    std::va_list args;
    va_start(args, format); //-V2528
    std::vsprintf(buffer, format, args);
    va_end(args);
    return Text::Draw(x, y, buffer);
}


int Text::DrawFormText(int x, int y, Color color, pString text, ...)
{
    color.SetAsCurrent();

#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    std::va_list args;
    va_start(args, text); //-V2528
    std::vsprintf(buffer, const_cast<char *>(text), args);
    va_end(args);
    return Text::Draw(x, y, buffer);
}
