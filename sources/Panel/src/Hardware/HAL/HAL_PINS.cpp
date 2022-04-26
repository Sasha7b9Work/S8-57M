// 2022/04/26 09:17:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f4xx_hal.h>


namespace HAL_PINS
{
    static const GPIO_TypeDef *ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI };

    static const uint16 pins[16] =
    {
        GPIO_PIN_0,
        GPIO_PIN_1,
        GPIO_PIN_2,
        GPIO_PIN_3,
        GPIO_PIN_4,
        GPIO_PIN_5,
        GPIO_PIN_6,
        GPIO_PIN_7,
        GPIO_PIN_8,
        GPIO_PIN_9,
        GPIO_PIN_10,
        GPIO_PIN_11,
        GPIO_PIN_12,
        GPIO_PIN_13,
        GPIO_PIN_14,
        GPIO_PIN_15
    };
}


struct PinLTDC : public Pin
{
    PinLTDC(int port, int pin) : Pin(PinMode::_LTDC, (Port)(port), (PinPin::E)(pin)) { }
};


void HAL_PINS::Init()
{
    PinLTDC(A, 3).Init();      // B5
}


void Pin::Init()
{
    GPIO_InitTypeDef isGPIO = { HAL_PINS::pins[pin] };

    isGPIO.Pull = GPIO_PULLUP;

    if (mode == PinMode::_LTDC)
    {
        isGPIO.Pull = GPIO_PULLDOWN;
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FREQ_LOW;
        isGPIO.Alternate = GPIO_AF14_LTDC;
    }

    HAL_GPIO_Init((GPIO_TypeDef *)(const_cast<GPIO_TypeDef *>(HAL_PINS::ports[port])), &isGPIO);
}
