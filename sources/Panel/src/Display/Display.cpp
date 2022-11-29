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


/*
void Display::ToggleBuffers(void)
{
    uint destination = reinterpret_cast<uint>(frontBuffer);
    uint source = reinterpret_cast<uint>(backBuffer);

    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = 0;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, source, destination, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}
*/


void Display::ClearBuffer(Color color)
{
    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_R2M;
    hDMA2D.Init.ColorMode = DMA2D_RGB888;
    hDMA2D.Init.OutputOffset = 0;

//    hDMA2D.XferCpltCallback = 0;
//
//    hDMA2D.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
//    hDMA2D.LayerCfg[0].InputAlpha = 0xFF;
//    hDMA2D.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
//    hDMA2D.LayerCfg[0].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 0) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, COLOR(color.value), (uint)buffer, Display::WIDTH, Display::HEIGHT) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
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


void Display::Update()
{
//    int x = (int)(std::rand() % Display::WIDTH);
//    int y = (int)(std::rand() % Display::HEIGHT);
//    int width = (int)(std::rand() % Display::WIDTH);
//    int height = (int)(std::rand() % Display::HEIGHT);
//
//    if (x + width < Display::WIDTH && y + height > Display::HEIGHT)
//    {
//        Painter::SetColor(Color(std::rand() % 32));
//
//        Painter::FillRegion(x, y, width, height);
//    }
//
//    HAL_Delay(2);

    Update1();
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

    Painter::DrawRectangle(x, y, width, width);

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

    Painter::SetColor(Color::CHAN[0]);

    Painter::DrawRectangle(0, 0, Display::WIDTH - 1, Display::HEIGHT - 1);

    Painter::SetColor(Color::WHITE);

    Painter::SetPoint(0, 0);
    Painter::SetPoint(Display::WIDTH - 1, 0);
    Painter::SetPoint(Display::WIDTH - 1, Display::HEIGHT - 1);
    Painter::SetPoint(0, Display::HEIGHT - 1);
}
