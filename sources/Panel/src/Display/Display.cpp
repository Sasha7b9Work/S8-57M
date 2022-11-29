#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Colors.h"
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
    LTDC_::Init(reinterpret_cast<uint>(buffer));
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

    Text::Draw(x0, y0, "���");

    Text::Draw(x0 + dX, y0, "�����");

    Text::Draw(x1, y1, "�8-57");

    Painter::EndScene();
}


void Display::Update()
{
    for (int i = 0; i < 32; i++)
    {
        COLOR(i) = (uint8)std::rand();
    }

    for (int x = 0; x < Display::WIDTH; x++)
    {
        for (int y = 0; y < Display::HEIGHT; y++)
        {
            Painter::SetColor(Color((uint8)std::rand()));
            Painter::SetPoint(x, y);
        }
    }

//    Painter::SetColor(Color::WHITE);

//    Painter::FillRegion(10, 10, 100, 100);
}
