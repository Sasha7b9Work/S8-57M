// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Display/BackBuffer.h"


uint Color::raw[256] =
{
    /* 0  */    MAKE_COLOR(0x00, 0x00, 0x00),       // BLACK
    /* 1  */    MAKE_COLOR(0xff, 0xff, 0xff),       // WHITE
    /* 2  */    MAKE_COLOR(0x60, 0x60, 0x60),       // GRID
    /* 3  */    MAKE_COLOR(0x00, 0xdf, 0xff),       // DATA_A
    /* 4  */    MAKE_COLOR(0x00, 0xff, 0x00),       // DATA_B
    /* 5  */    MAKE_COLOR(0x90, 0x90, 0xa0),       // MENU_FIELD
    /* 6  */    MAKE_COLOR(31 * 8, 44 * 4, 0 * 8), // MENU_TITLE
    /* 7  */    MAKE_COLOR(0x00, 0x00, 0x80),       // MENU_TITLE_DARK
    /* 8  */    MAKE_COLOR(31 * 8, 63 * 4, 0 * 8), // MENU_TITLE_BRIGHT
    /* 9  */    MAKE_COLOR(0x20, 0x45, 0x20),       // BATTERY_EMPTY
    /* 10 */    MAKE_COLOR(13 * 8, 17 * 4, 0 * 8), // MENU_ITEM_DARK
    /* 11 */    MAKE_COLOR(31 * 8, 51 * 4, 0 * 8), // MENU_ITEM_BRIGHT
    /* 12 */    MAKE_COLOR(0x00, 13,   0x00),       // DATA_WHITE_ACCUM_A
    /* 13 */    MAKE_COLOR(0x00, 25,   0x00),       // DATA_WHITE_ACCUM_B
    /* 14 */    MAKE_COLOR(0xff, 0x00, 0x00),       // RED
    /* 15 */    MAKE_COLOR(0x00, 0xff, 0x00),       // GREEN
    /* 16 */    MAKE_COLOR(0x00, 0x00, 0xff),       // BLUE
    /* 17 */    MAKE_COLOR(0x00, 0x00, 0x40),
    /* 18 */    MAKE_COLOR(0x00, 0x00, 0x80),
    /* 19 */    MAKE_COLOR(0x1a, 0x1a, 0x1a),       // GRAY_10
    /* 20 */    MAKE_COLOR(0x33, 0x33, 0x33),       // GRAY_20
    /* 21 */    MAKE_COLOR(0x00, 0x00, 0x1a),       // BLUE_10
    /* 22 */    MAKE_COLOR(0x80, 0x80, 0x80),       // GRAY_50
    /* 23 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // GRAY_75
    /* 24 */    MAKE_COLOR(0x00, 0x6f, 0x80),       // DATA_HALF_A
    /* 25 */    MAKE_COLOR(0x00, 0x80, 0x00),       // DATA_HALF_B
    /* 26 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // SEPARATOR
    /* 27 */    MAKE_COLOR(0xff, 0xff, 0x00),       // YELLOW
    /* 28 */    MAKE_COLOR(0x00, 0x7f, 0x00),       // BATTERY
    /* 29 */    MAKE_COLOR(0x00, 0x4c, 0x00)        // GRAY_30
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
