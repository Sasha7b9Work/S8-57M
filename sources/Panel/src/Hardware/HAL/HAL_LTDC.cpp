// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/SettingsTypes.h"


static LTDC_HandleTypeDef handleLTDC;
static uint frontBuffer = 0;



void HAL_LTDC::Init(uint front)
{
    GPIO_InitTypeDef isGPIO =
    {
        //  R3         R6
        GPIO_PIN_0 | GPIO_PIN_1,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        GPIO_AF9_LTDC
    };
    HAL_GPIO_Init(GPIOB, &isGPIO); //-V2571

    //              B5          VSYNC         G2            R4          R5
    isGPIO.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &isGPIO); //-V2571

    //              G4             G5            B6          B7
    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &isGPIO); //-V2571

    //              HSYNC         G6           R2
    isGPIO.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOC, &isGPIO); //-V2571

    //                B3          G7          B2
    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &isGPIO); //-V2571

    //               G3             B4           DE            CLK           R7
    isGPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &isGPIO); //-V2571

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_5;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &initStr); //-V2571

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);     // ��������� ��������� //-V2571

    //                R/L         U/D
    initStr.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC, &initStr); //-V2571
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);      // ����� �������������� ���������� //-V2571
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);        // ����� ������������ ���������� //-V2571


    handleLTDC.Instance = LTDC; //-V2571
    handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    handleLTDC.Init.HorizontalSync = 0;
    handleLTDC.Init.VerticalSync = 0;
    handleLTDC.Init.AccumulatedHBP = 70;
    handleLTDC.Init.AccumulatedVBP = 13;
    handleLTDC.Init.AccumulatedActiveW = 390;
    handleLTDC.Init.AccumulatedActiveH = 253;
    handleLTDC.Init.TotalWidth = 408;
    handleLTDC.Init.TotalHeigh = 263;
    handleLTDC.Init.Backcolor.Blue = 255;
    handleLTDC.Init.Backcolor.Green = 255;
    handleLTDC.Init.Backcolor.Red = 255;

    if (HAL_LTDC_Init(&handleLTDC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    frontBuffer = front;

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
    pLayerCfg.FBStartAdress = frontBuffer;
    pLayerCfg.ImageWidth = 320;
    pLayerCfg.ImageHeight = 240;
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

    Painter::LoadPalette();
}


void HAL_LTDC::SetColors(uint *clut, uint numColors)
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, numColors, 0);

    HAL_LTDC_EnableCLUT(&handleLTDC, 0);
}
