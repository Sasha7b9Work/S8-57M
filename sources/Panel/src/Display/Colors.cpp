// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Display/BackBuffer.h"


uint Color::raw[256];


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
