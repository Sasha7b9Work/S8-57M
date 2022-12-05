// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include <cstdlib>



namespace Display
{
    LTDC_HandleTypeDef hltdc;

    uint8  front[Display::WIDTH * Display::HEIGHT];

    uint8 *buffer = front;

    static void DrawStartScreen();
}


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init(reinterpret_cast<uint>(buffer));
    Painter::LoadPalette();

    DrawStartScreen();
}


void Display::ClearBuffer()
{
    FillRegion(320, 240, Display::WIDTH / 2, Display::HEIGHT / 2);
}


void Display::FillRegion(const int x, const int y, const int w, const int h)
{
    Color color = Painter::GetColor();

    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_R2M;
    hDMA2D.Init.ColorMode = DMA2D_ARGB8888;
    hDMA2D.Init.OutputOffset = (uint)(x + y * Display::WIDTH) / 4;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        uint pdata = (uint)(color.value | (color.value << 8) | (color.value << 16) | (color.value << 24));
        uint dest = (uint)buffer;
        uint width = w / 4U;
        uint height = (uint)h;

        if (HAL_DMA2D_Start(&hDMA2D, pdata, dest, width, height) == HAL_OK)
        {
            HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
        }
    }
}


uint8 *Display::GetBuffer()
{
    return buffer;
}


uint8 *Display::GetBufferEnd()
{
    return buffer + Display::WIDTH * Display::HEIGHT;
}


void Display::DrawStartScreen()
{
    Painter::SetColor(Color::BACK);

    ClearBuffer();

    Painter::SetColor(Color::FILL);

    PFont::Set(PTypeFont::_GOST28);
    Text::SetSpacing(3);

    int x0 = 85;
    int dX = 65;
    int y0 = 50;

    int x1 = 120;
    int y1 = 130;

    Text::Draw(x0, y0, "ÎÀÎ");

    Text::Draw(x0 + dX, y0, "ÌÍÈÏÈ");

    Text::Draw(x1, y1, "Ñ8-57");

    Painter::EndScene();
}


uint Display::Update()
{
    uint start = TIME_MS;

    ClearBuffer();

//    Painter::SetColor(Color::BLACK);
//
//    Painter::FillRegion(0, 0, Display::WIDTH, Display::HEIGHT);

    Update1();

    return TIME_MS - start;
}


void Display::Update1()
{
    static const int width = 100;
    static int x = 0;
    static int y = 0;

    static int dX = 1;
    static int dY = 1;

    Painter::SetColor(Color::WHITE);

    Painter::FillRegion(x + 1, y + 1, width - 2, width - 2);

    Painter::SetColor(Color::BLACK);

//    Painter::DrawRectangle(x, y, width, width);

    x += dX;
    y += dY;

    if (x > Display::WIDTH - width)
    {
        dX = -1;
        x += dX;
    }

    if (x < 0)
    {
        dX = 1;
        x += dX;
    }

    if (y > Display::HEIGHT - width)
    {
        dY = -1;
        y += dY;
    }

    if (y < 0)
    {
        dY = 1;
        y += dY;
    }

    Painter::SetColor(Color::WHITE);

    Painter::SetPoint(0, 0);
    Painter::SetPoint(Display::WIDTH - 1, 0);
    Painter::SetPoint(Display::WIDTH - 1, Display::HEIGHT - 1);
    Painter::SetPoint(0, Display::HEIGHT - 1);
}
