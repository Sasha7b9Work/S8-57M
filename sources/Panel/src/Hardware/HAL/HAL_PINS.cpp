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
    PinLTDC(int port, int pin) : Pin(PinMode::_LTDC, (Port)(port), pin) { }
};


PinOut::PinOut(Port _port, int pin) : Pin(PinMode::_Output, _port, pin)
{
    Init();
}


PinIn::PinIn(Port _port, int pin) : Pin(PinMode::_Input, _port, pin)
{
    Init();
}


void HAL_PINS::Init()
{
    PinLTDC(A, 4).Init();       // VSYNC
    PinLTDC(C, 6).Init();       // HSYNC
    PinLTDC(E, 13).Init();      // DE
    PinLTDC(E, 14).Init();      // CKL

    PinLTDC(C, 10).Init();      // R2
    PinLTDC(B, 0).Init();       // R3
    PinLTDC(A, 11).Init();      // R4
    PinLTDC(A, 12).Init();      // R5
    PinLTDC(B, 6).Init();       // R6
    PinLTDC(E, 15).Init();      // R7

    PinLTDC(A, 6).Init();       // G2
    PinLTDC(E, 11).Init();      // G3
    PinLTDC(B, 10).Init();      // G4
    PinLTDC(B, 11).Init();      // G5
    PinLTDC(C, 7).Init();       // G6
    PinLTDC(D, 3).Init();       // G7

    PinLTDC(D, 6).Init();       // B2
    PinLTDC(D, 10).Init();      // B3
    PinLTDC(E, 12).Init();      // B4
    PinLTDC(A, 3).Init();       // B5
    PinLTDC(B, 8).Init();       // B6
    PinLTDC(B, 9).Init();       // B7

    PinOut pinDisplayOn(A, 5);
    pinDisplayOn.Set();

    PinOut pinRL(C, 11);         // Горизонтальная ориентация дисплея
    PinOut pinUD(C, 12);         // Вертикальная ориентация дисплея

    pinRL.Reset();
    pinUD.Set();
}


void Pin::Init()
{
    GPIO_InitTypeDef isGPIO = { HAL_PINS::pins[pin] };

    isGPIO.Pull = GPIO_PULLUP;

    if (mode == PinMode::_Input)
    {
        isGPIO.Mode = GPIO_MODE_INPUT;
    }
    else if (mode == PinMode::_Output)
    {
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Speed = GPIO_SPEED_HIGH;
    }
    else if (mode == PinMode::_LTDC)
    {
        isGPIO.Pull = GPIO_PULLDOWN;
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FREQ_LOW;
        isGPIO.Alternate = GPIO_AF14_LTDC;
    }

    HAL_GPIO_Init((GPIO_TypeDef *)(const_cast<GPIO_TypeDef *>(HAL_PINS::ports[port])), &isGPIO);
}


void Pin::Set()
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)HAL_PINS::ports[port], HAL_PINS::pins[pin], GPIO_PIN_SET);
}


void Pin::Reset()
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)HAL_PINS::ports[port], HAL_PINS::pins[pin], GPIO_PIN_RESET);
}
