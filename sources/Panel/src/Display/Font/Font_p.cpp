// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "AdvancedFont_p.h"
#include "common/Command.h"
#include "Hardware/HAL/HAL.h"
#include "Font8.h"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#include <cstring>


static const Font *font8 = (const Font *)font_gost_type_a_14;


const Font *Font::fonts[TypeFont::Count] = {&font5, font8, &fontUGO, &fontUGO2, nullptr};
const Font *Font::font = font8;

TypeFont::E pushedFont = TypeFont::_8;
TypeFont::E currentFont = TypeFont::_8;


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
            font = &font5;
            break;
        case TypeFont::_8:
            font = font8;
            break;
        case TypeFont::_UGO:
            font = &fontUGO;
            break;
        case TypeFont::_UGO2:
            font = &fontUGO2;
            break;
        case TypeFont::_GOST28:
        case TypeFont::_GOST72bold:
        case TypeFont::_OMEGA72:
        {
            font = nullptr;
            volatile PAdvancedFont f(typeFont);
        }       
            break;
        case TypeFont::None:
        case TypeFont::Count:
            break;
        }

        currentFont = typeFont;
    }
}


void Font::Pop()
{
    Set(pushedFont);
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

    return PAdvancedFont().GetWidth(symbol);
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth((uint8)(symbol));
}


uint8 Font::GetHeight()
{
    if (FontIsSmall())
    {
        return (uint8)(font->_height);
    }

    return PAdvancedFont().GetHeight();
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return PAdvancedFont().RowNotEmpty(symbol, row);
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return PAdvancedFont().BitIsExist(symbol, row, bit);
}


bool Font::IsBig()
{
    return !FontIsSmall();
}
