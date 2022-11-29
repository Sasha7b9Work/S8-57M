#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handlerTIM2 = { TIM2 };


void HAL_TIM2::Init(uint prescaler, uint period)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handlerTIM2.Init.Prescaler = prescaler;
    handlerTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
    handlerTIM2.Init.Period = period;
    handlerTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_DeInit(&handlerTIM2);
}


void HAL_TIM2::Start()
{
    HAL_TIM_Base_Start(&handlerTIM2);
}


void HAL_TIM2::Stop()
{
    HAL_TIM_Base_Stop(&handlerTIM2);
}


void HAL_TIM2::DeInit()
{
    __HAL_RCC_TIM2_CLK_DISABLE();
}


uint HAL_TIM2::TimeUS()
{
    return TIM2->CNT / 90;
}


uint HAL_TIM2::TimeTicks()
{
    return TIM2->CNT;
}


void HAL_TIM2::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}
