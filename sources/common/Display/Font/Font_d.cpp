// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font.h"
#include "AdvancedFont_d.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Font8.h"
#ifndef LOADER
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#endif
#include <cstring>


static const Font *font8 = (const Font *)font_gost_type_a_14;


#ifdef LOADER
const Font *Font::fonts[TypeFont::Count] = {font8, font8, font8, font8, nullptr};
#else
const Font *Font::fonts[TypeFont::Count] = {&font5, font8, &fontUGO, &fontUGO2, nullptr};
#endif
const Font *Font::font = font8;

TypeFont::E pushedFont = TypeFont::_8;
TypeFont::E currentFont = TypeFont::_8;

static int spacing = 1;


int Font::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = (char *)text;

    while(*symbol)
    {
        result += GetWidth(*symbol) + spacing; //-V1026
        symbol++;
    }
    return result * 2;
}


static void SendTypeFontToPanel(TypeFont::E type)
{
    static TypeFont::E prevType = TypeFont::Count;

    if (prevType != type)
    {
        SBuffer(Command::Paint_SetFont, (uint8)type).Send();
        prevType = type;
    }
}


TypeFont::E Font::Current()
{
    return currentFont;
}


void Font::Set(const TypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case TypeFont::_5:
#ifndef LOADER
            font = &font5;
#endif
            break;
        case TypeFont::_8:
            font = font8;
            break;
        case TypeFont::_UGO:
#ifndef LOADER
            font = &fontUGO;
#endif
            break;
        case TypeFont::_UGO2:
#ifndef LOADER
            font = &fontUGO2;
#endif
            break;
        case TypeFont::_GOST28:
        case TypeFont::_GOST72bold:
        {
            font = nullptr;
            volatile DAdvancedFont f(typeFont);
        }       
            break;
        case TypeFont::None:
        case TypeFont::_OMEGA72:
        case TypeFont::Count:
            break;
        }

        SendTypeFontToPanel(typeFont);

        currentFont = typeFont;
    }
}


void Font::Pop()
{
    Set(pushedFont);
}


void Font::SetSpacing(int _spacing)
{
    spacing = _spacing;
    SBuffer(Command::Paint_SetTextSpacing, (uint8)spacing).Send();
}

int Font::GetSpacing()
{
    return spacing;
}


void Font::SetMinWidth(uint8 width)
{
    SBuffer(Command::Paint_SetMinWidthFont, width).Send();
}


static bool FontIsSmall()
{
    return currentFont <= TypeFont::_UGO2;
}


uint8 Font::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return DAdvancedFont().GetWidth(symbol);
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth((uint8)(symbol));
}


uint8 Font::GetHeight()
{
    if (FontIsSmall())
    {
        return (uint8)(font->_height * 2);
    }

    return DAdvancedFont().GetHeight();
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return DAdvancedFont().RowNotEmpty(symbol, row);
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return DAdvancedFont().BitIsExist(symbol, row, bit);
}


bool Font::IsBig()
{
    return !FontIsSmall();
}
