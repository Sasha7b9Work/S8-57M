// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


uint8  front[Display::WIDTH * Display::HEIGHT];


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init(reinterpret_cast<uint>(front));
    Painter::LoadPalette();
}
