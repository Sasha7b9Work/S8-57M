#include "defines.h"
#include "AdvancedFont_p.h"
#include "common/Command.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#include <cstring>


const PFont *PFont::fonts[PTypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const PFont *PFont::font = &font8;

PTypeFont::E pushedFont = PTypeFont::_8;
PTypeFont::E currentFont = PTypeFont::_8;


PTypeFont::E PFont::Current()
{
    return currentFont;
}


void PFont::Set(const PTypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case PTypeFont::_5:
            font = &font5;
            break;
        case PTypeFont::_8:
            font = &font8;
            break;
        case PTypeFont::_UGO:
            font = &fontUGO;
            break;
        case PTypeFont::_UGO2:
            font = &fontUGO2;
            break;
        case PTypeFont::_GOST28:
        case PTypeFont::_GOST72bold:
        case PTypeFont::_OMEGA72:
        {
            font = nullptr;
            volatile PAdvancedFont f(typeFont);
        }       
            break;
        case PTypeFont::None:
        case PTypeFont::Count:
            break;
        }

        currentFont = typeFont;
    }
}


void PFont::Pop()
{
    Set(pushedFont);
}


static bool FontIsSmall()
{
    return currentFont <= PTypeFont::_UGO2;
}


uint8 PFont::GetWidth(uint8 symbol) //-V2506
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return PAdvancedFont().GetWidth(symbol);
}


uint8 PFont::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 PFont::GetHeight() //-V2506
{
    if (FontIsSmall())
    {
        return static_cast<uint8>(font->_height);
    }

    return PAdvancedFont().GetHeight();
}


bool PFont::RowNotEmpty(uint8 symbol, int row) //-V2506
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return PAdvancedFont().RowNotEmpty(symbol, row);
}


bool PFont::BitIsExist(uint8 symbol, int row, int bit) //-V2506
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return PAdvancedFont().BitIsExist(symbol, row, bit);
}


bool PFont::IsBig()
{
    return !FontIsSmall();
}
