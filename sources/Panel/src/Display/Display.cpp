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


static void DrawStartScreen();


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init(reinterpret_cast<uint>(frontBuffer)); //-V2571
    Painter::LoadPalette();

    DrawStartScreen();
}


uint8 *Display::GetBuffer()
{
    return frontBuffer;
}


uint8 *Display::GetBufferEnd()
{
    return frontBuffer + Display::WIDTH * Display::HEIGHT; //-V2563
}


static void DrawStartScreen()
{
    Painter::SetColor(Color::BACK);

    Painter::BeginScene();

    Painter::SetColor(Color::FILL);

    PFont::Set(PTypeFont::_GOST28);
    Text::SetSpacing(3);

    int x0 = 85;
    int dX = 65;
    int y0 = 50;

    int x1 = 120;
    int y1 = 130;

    Text::Draw(x0, y0, "Œ¿Œ");

    Text::Draw(x0 + dX, y0, "ÃÕ»œ»");

    Text::Draw(x1, y1, "—8-57");

    Painter::EndScene();
}
