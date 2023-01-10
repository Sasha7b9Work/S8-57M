// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display.h"
#include "Display/Text/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Display/BackBuffer.h"
#include "Display/Font/Font_p.h"
#include <cstdlib>
#include <cstring>


namespace Display
{
    LTDC_HandleTypeDef hltdc;

    uint8  front[Display::WIDTH * Display::HEIGHT];

    uint8 *buffer = front;

    static void DrawStartScreen();
}


void Display::SetField(uint offset, uint8 *data, int num_bytes)
{
    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = NULL;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, (uint)data, (uint)(front), 640, 480 / 5) == HAL_OK)
            {
                // \todo с какого-то хрена __HAL_DMA2D_ENABLE() в HAL_DMA2D_Start не заводит флаг DMA2D_CR_START
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }

    std::memcpy(front + offset, data, (uint)num_bytes);
}


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init((uint)buffer);
    BackBuffer::LoadPalette();

    DrawStartScreen();
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
    for (int field = 0; field < 5; field++)
    {
        BackBuffer::BeginPaint(field);

        PFont::Set(PTypeFont::_GOST28);
        Text::SetSpacing(3);

        int x0 = 85;
        int dX = 65;
        int y0 = 50;

        int x1 = 120;
        int y1 = 130;

        Text::Draw(x0, y0, "ОАО");

        Text::Draw(x0 + dX, y0, "МНИПИ");

        Text::Draw(x1, y1, "С8-57");

        BackBuffer::EndPaint();
    }
}


uint Display::Update()
{
    uint start = TIME_MS;

    for (int field = 0; field < 5; field++)
    {
        BackBuffer::BeginPaint(field);

        Update1();

        BackBuffer::EndPaint();
    }

    return TIME_MS - start;
}


void Display::Update1()
{
    static const int width = 100;
    static int x = 0;
    static int y = 0;

    static int dX = 1;
    static int dY = 1;

    BackBuffer::FillRegion(x + 1, y + 1, width - 2, width - 2);


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

    for (int i = 10; i < 100; i += 10)
    {
        BackBuffer::SetPixel(i, 10);
    }
}
