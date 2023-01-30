// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Display/BackBuffer.h"


uint Color::raw[256] =
{
    MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
    MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
    MAKE_COLOR(0x60, 0x60, 0x60),   // GRID
    MAKE_COLOR(0x00, 0xdf, 0xff),   // DATA_A
    MAKE_COLOR(0x00, 0xff, 0x00),   // DATA_B
    MAKE_COLOR(0x90, 0x90, 0xa0),   // MENU_FIELD
    MAKE_COLOR(0xf8, 0xb0, 0x00),   // MENU_TITLE
    MAKE_COLOR(0x00, 0x00, 0x80),   // MENU_TITLE_DARK
    MAKE_COLOR(0xf8, 0xfc, 0x00),   // MENU_TITLE_BRIGHT
    MAKE_COLOR(0x68, 0x44, 0x00),   // MENU_ITEM_DARK
    MAKE_COLOR(0xf8, 0xcc, 0x00),   // MENU_ITEM_BRIGHT
    MAKE_COLOR(0xff, 0x00, 0x00),   // RED
    MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
    MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
    MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
    MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
    MAKE_COLOR(0x1a, 0x1a, 0x1a),   // GRAY_10
    MAKE_COLOR(0x33, 0x33, 0x33),   // GRAY_20
    MAKE_COLOR(0x00, 0x00, 0x1a),   // BLUE_10
    MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
    MAKE_COLOR(0xc0, 0xc0, 0xc0),   // GRAY_75
    MAKE_COLOR(0xc0, 0xc0, 0xc0),   // SEPARATOR
    MAKE_COLOR(0xff, 0xff, 0x00),   // YELLOW
    MAKE_COLOR(0x00, 0x7f, 0x00),   // BATTERY
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
/* 251 */    0x3F00,
/* 252 */    0x373F,
/* 253 */    0x7F00,
/* 254 */    0x6F7F,
/* 255 */    0
};


Color Color::current(255);


bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}


bool operator==(const Color &left, const Color &right)
{
    return left.value == right.value;
}


bool operator>(const Color &left, const Color &right)
{
    return left.value > right.value;
}


void Color::SetValue(uint _raw)
{
    raw[value] = _raw;

    BackBuffer::LoadPalette();
}
