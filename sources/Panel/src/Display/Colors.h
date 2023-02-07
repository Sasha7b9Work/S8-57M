// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"

#define COLOR(x) Color::raw[x]


class Color
{
public:

    explicit Color(uint8 val = 0) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    void SetValue(uint raw);
    void SetAsCurrent() const { if (value != (uint8)(-1)) { current = *this; } }

    uint8 value;
    static Color current;
    static uint raw[256];
};


bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);


#define MAKE_COLOR(r, g, b) ((uint)((uint)(b) + ((uint)(g) << 8) + ((uint)(r) << 16)))
#define R_FROM_COLOR(color) (((uint)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((uint)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((uint)(color))       & 0xff)
