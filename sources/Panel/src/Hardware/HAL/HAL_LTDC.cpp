// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"


namespace HAL_LTDC
{
    static LTDC_HandleTypeDef handleLTDC;
    static uint buffer = 0;

    static void SetBuffer(uint buffer);
}


void HAL_LTDC::Init(uint _buffer)
{
    GPIO_InitTypeDef is =
    {
        //  R5           G6           DEN
        GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_9,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        GPIO_AF14_LTDC
    };
    HAL_GPIO_Init(GPIOC, &is);

    //           R2           B5           G2           R4
    is.Pin = GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOA, &is);

    //           R3           R6           G4            G5            B6           B7
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &is);

    //           G3            B4            DE            CLK           R7
    is.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &is);

    //           B3            G7           B2
    is.Pin = GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &is);

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_5;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &initStr);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);     // ��������� ���������

    //                R/L         U/D
    initStr.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOA, &initStr);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);      // ����� �������������� ����������
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);        // ����� ������������ ����������


    handleLTDC.Instance = LTDC;
    handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    handleLTDC.Init.HorizontalSync = 0;
    handleLTDC.Init.VerticalSync = 0;
    handleLTDC.Init.AccumulatedHBP = 160;
    handleLTDC.Init.AccumulatedVBP = 45;
    handleLTDC.Init.AccumulatedActiveW = 800;
    handleLTDC.Init.AccumulatedActiveH = 525;
    handleLTDC.Init.TotalWidth = 800;
    handleLTDC.Init.TotalHeigh = 525;
    handleLTDC.Init.Backcolor.Blue = 255;
    handleLTDC.Init.Backcolor.Green = 255;
    handleLTDC.Init.Backcolor.Red = 255;
    if (HAL_LTDC_Init(&handleLTDC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    SetBuffer(_buffer);

    COLOR(0) = 0;
    COLOR(1) = 0x00ffffff;
    COLOR(2) = 0x00a0a0a0;
    COLOR(3) = 0x000000ff;

    Painter::LoadPalette();
}


void HAL_LTDC::SetBuffer(uint _buffer)
{
    buffer = _buffer;

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = Display::WIDTH;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = Display::HEIGHT;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 255;
    pLayerCfg.Alpha0 = 255;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = buffer;
    pLayerCfg.ImageWidth = Display::WIDTH;
    pLayerCfg.ImageHeight = Display::HEIGHT;
    pLayerCfg.Backcolor.Blue = 128;
    pLayerCfg.Backcolor.Green = 128;
    pLayerCfg.Backcolor.Red = 0;

    if (HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


void HAL_LTDC::SetColors(uint *clut, uint numColors)
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, numColors, 0);

    HAL_LTDC_EnableCLUT(&handleLTDC, 0);
}


/*
void LTDC_::ToggleBuffers()
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
            if (HAL_DMA2D_Start(&hDMA2D, backBuffer, frontBuffer, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}
*/
