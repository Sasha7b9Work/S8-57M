#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


// ��� ������� ���������� ������������
static ADC_HandleTypeDef handle;
// ��� ������������
static ADC_ChannelConfTypeDef config =
{
    ADC_CHANNEL_2,
    1,
    ADC_SAMPLETIME_3CYCLES,
    0
};


static void SetActiveChannel2();
static void SetActiveChannel9();
static uint ReadValue();


void HAL_ADC1::Init()
{
    // �������� ����� ��� ��� �������� ����������
    // 36 : PA2 - ADC1 IN2 - �������� ���
    // 47 : PB1 - ADC1 IN9 - �������� ���������
    // ����� ������:

    __ADC1_CLK_ENABLE();

    HAL_PIO::Init(PIN_ADC1_IN2, HMode::Analog, HPull::No);
    HAL_PIO::Init(PIN_ADC1_IN9, HMode::Analog, HPull::No);

    handle.Instance = ADC1;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION_12B;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;  // ����������� ������ �������������� �������
    handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&handle);

    SetActiveChannel2();
}


uint HAL_ADC1::ValueBattery()
{
    SetActiveChannel2();
    
    return ReadValue();
}


uint HAL_ADC1::ValueCharger()
{
    SetActiveChannel9();

    return ReadValue();
}


static uint ReadValue()
{
    HAL_ADC_Start(&handle);

    do
    {
    } while (HAL_ADC_PollForConversion(&handle, 1) != HAL_OK);

    return HAL_ADC_GetValue(&handle);
}


static void SetActiveChannel2()
{
    config.Channel = ADC_CHANNEL_2;

    HAL_ADC_ConfigChannel(&handle, &config);
}


static void SetActiveChannel9()
{
    config.Channel = ADC_CHANNEL_9;

    HAL_ADC_ConfigChannel(&handle, &config);
}
