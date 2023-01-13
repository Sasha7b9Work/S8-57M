// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Font8.h"

#ifndef LOADER
    #include "Font5.h"
    #include "fontUGO.inc"
    #include "FontUGO.h"

static const Font *font5 = (const Font *)font_gost_type_a_10;
static const Font *fontUGO2 = (const Font *)font_UGO;

#endif

#include <cstring>

static const Font *font8 = (const Font *)font_gost_type_a_14;
static const Font *font = font8;


#ifdef LOADER
    const Font *Font::fonts[TypeFont::Count] = { font8, font8, font8, font8 };
#else
    const Font *Font::fonts[TypeFont::Count] = { font5, font8, &fontUGO, fontUGO2 };
#endif

static TypeFont::E pushedFont = TypeFont::Normal;
static TypeFont::E currentFont = TypeFont::Normal;

static int spacing = 1;


int Font::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = (char *)text;

    while(*symbol)
    {
        result += Symbol(*symbol).Width() + spacing; //-V1026
        symbol++;
    }
    return result;
}


#ifndef PANEL
static void SendTypeFontToPanel(TypeFont::E type)
{
    static TypeFont::E prevType = TypeFont::Count;

    if (prevType != type)
    {
        SBuffer(Command::Paint_SetFont, (uint8)type).Send();
        prevType = type;
    }
}
#endif


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
        case TypeFont::Small:
#ifndef LOADER
            font = font5;
#endif
            break;
        case TypeFont::Normal:
            font = font8;
            break;
        case TypeFont::Diagram:
#ifndef LOADER
            font = &fontUGO;
#endif
            break;
        case TypeFont::UGO:
#ifndef LOADER
            font = fontUGO2;
#endif
            break;
        case TypeFont::None:
        case TypeFont::Count:
            break;
        }

#ifndef PANEL
        SendTypeFontToPanel(typeFont);
#endif

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


int Font::Height()
{
    static const int h[TypeFont::Count] =
    {
        14,
        20,
        10,
        10
    };

    return h[currentFont];
}


bool SymbolRow::BitIsExist(int bit) const
{
    return row & (1 << bit);
}


SymbolRow::SymbolRow(uint16 _row) : row(_row) {}


SymbolRow Symbol::GetRow(int r) const
{
    uint16 *size = (uint16 *)font + symbol * (Font::Height() + 1);

    uint16 *row = size + 1 + r;

    return SymbolRow(*row);
}


int Symbol::Width() const
{
    uint16 *size = (uint16 *)font + symbol * (Font::Height() + 1);

    return *size;
}
