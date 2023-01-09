// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"

extern uint colors[256];

#define COLOR(x) colors[x]


class Color
{
public:

    explicit Color(uint8 val = 0) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    void SetValue(uint raw);
    void SetAsCurrent() const { if (value != (uint8)(-1)) { current = *this; } };
    static Color Current() { return current; }

    static Color Trig();

    uint8 value;

private:

    static Color current;
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4623)
#endif


#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false, false};

#ifdef _WIN32
#pragma warning(pop)
#endif

#define MAKE_COLOR(r, g, b) ((uint)((uint)(b) + ((uint)(g) << 8) + ((uint)(r) << 16)))
#define R_FROM_COLOR(color) (((uint)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((uint)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((uint)(color))       & 0xff)
