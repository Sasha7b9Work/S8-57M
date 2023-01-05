// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_CRC32
{
    static CRC_HandleTypeDef handle = { CRC };
}


void HAL_CRC32::Init()
{
    __HAL_RCC_CRC_CLK_ENABLE();

    if (HAL_CRC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint HAL_CRC32::Calculate8bit(uint8 *buffer, int size)
{
    if((size % 4) != 0)
    {
        size -= (size % 4);
    }

    return HAL_CRC_Calculate(&handle, (uint *)buffer, (uint)(size / 4));
}
