// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Font_d.h"
#include "AdvancedFont_d.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#ifndef LOADER
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#endif
#include <cstring>


#ifdef LOADER
const DFont *DFont::fonts[DTypeFont::Count] = {&font8, &font8, &font8, &font8, nullptr};
#else
const DFont *DFont::fonts[DTypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
#endif
const DFont *DFont::font = &font8;

DTypeFont::E pushedFont = DTypeFont::_8;
DTypeFont::E currentFont = DTypeFont::_8;

static int spacing = 1;


int DFont::GetLengthText(pString text)
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


static void SendTypeFontToPanel(DTypeFont::E type)
{
    static DTypeFont::E prevType = DTypeFont::Count;

    if (prevType != type)
    {
        SBuffer(Command::Paint_SetFont, (uint8)type).Send();
        prevType = type;
    }
}


DTypeFont::E DFont::Current()
{
    return currentFont;
}


void DFont::Set(const DTypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case DTypeFont::_5:
#ifndef LOADER
            font = &font5;
#endif
            break;
        case DTypeFont::_8:
            font = &font8;
            break;
        case DTypeFont::_UGO:
#ifndef LOADER
            font = &fontUGO;
#endif
            break;
        case DTypeFont::_UGO2:
#ifndef LOADER
            font = &fontUGO2;
#endif
            break;
        case DTypeFont::_GOST28:
        case DTypeFont::_GOST72bold:
        {
            font = nullptr;
            volatile DAdvancedFont f(typeFont);
        }       
            break;
        case DTypeFont::None:
        case DTypeFont::Count:
            break;
        }

        SendTypeFontToPanel(typeFont);

        currentFont = typeFont;
    }
}


void DFont::Pop()
{
    Set(pushedFont);
}


void DFont::SetSpacing(int _spacing)
{
    spacing = _spacing;
    SBuffer(Command::Paint_SetTextSpacing, (uint8)spacing).Send();
}

int DFont::GetSpacing()
{
    return spacing;
}


void DFont::SetMinWidth(uint8 width)
{
    SBuffer(Command::Paint_SetMinWidthFont, width).Send();
}


static bool FontIsSmall()
{
    return currentFont <= DTypeFont::_UGO2;
}


uint8 DFont::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return DAdvancedFont().GetWidth(symbol);
}


uint8 DFont::GetWidth(char symbol)
{
    return GetWidth((uint8)(symbol));
}


uint8 DFont::GetHeight()
{
    if (FontIsSmall())
    {
        return (uint8)(font->_height * 2);
    }

    return DAdvancedFont().GetHeight();
}


bool DFont::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return DAdvancedFont().RowNotEmpty(symbol, row);
}


bool DFont::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return DAdvancedFont().BitIsExist(symbol, row, bit);
}


bool DFont::IsBig()
{
    return !FontIsSmall();
}
