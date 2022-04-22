// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>



LTDC_HandleTypeDef hltdc;

uint8  front[Display::WIDTH * Display::HEIGHT];
uint8 *Display::frontBuffer = front;


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init(reinterpret_cast<uint>(frontBuffer));
    Painter::LoadPalette();
}


uint8 *Display::GetBufferEnd()
{
    return frontBuffer + Display::WIDTH * Display::HEIGHT;
}
