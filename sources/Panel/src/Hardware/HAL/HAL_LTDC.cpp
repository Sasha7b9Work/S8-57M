// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/SettingsTypes.h"
#include "Display/Colors.h"


static LTDC_HandleTypeDef handleLTDC;
static uint8  front[DISPLAY_WIDTH * DISPLAY_HEIGHT];



void HAL_LTDC::Init()
{
    HAL_DAC2::Init();

    handleLTDC.Instance = LTDC;
    handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    handleLTDC.Init.HorizontalSync = 0;
    handleLTDC.Init.VerticalSync = 0;
    handleLTDC.Init.AccumulatedHBP = 70 * 2;
    handleLTDC.Init.AccumulatedVBP = 13 * 2;
    handleLTDC.Init.AccumulatedActiveW = 390 * 2;
    handleLTDC.Init.AccumulatedActiveH = 253 * 2;
    handleLTDC.Init.TotalWidth = 408 * 2;
    handleLTDC.Init.TotalHeigh = 263 * 2;
    handleLTDC.Init.Backcolor.Blue = 255;
    handleLTDC.Init.Backcolor.Green = 255;
    handleLTDC.Init.Backcolor.Red = 255;

    if (HAL_LTDC_Init(&handleLTDC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 320;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 240;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 255;
    pLayerCfg.Alpha0 = 255;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = (uint)front;
    pLayerCfg.ImageWidth = DISPLAY_WIDTH;
    pLayerCfg.ImageHeight = DISPLAY_HEIGHT;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;

    if (HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    COLOR(0) = 0;
    COLOR(1) = 0x00ffffff;
    COLOR(2) = 0x00a0a0a0;
    COLOR(3) = 0x000000ff;

    LoadPalette();
}


void HAL_LTDC::LoadPalette()
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, &COLOR(0), Color::NUMBER.value, 0);

    HAL_LTDC_EnableCLUT(&handleLTDC, 0);
}


void HAL_LTDC::CopyImage(uint8 *image, int x, int y, int width, int height)
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

    hDMA2D.Instance = DMA2D; //-V2571

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, (uint)image, (uint)(front + DISPLAY_WIDTH * y + x), (uint)width, (uint)height) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}
