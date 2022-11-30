// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include <stm32f4xx_hal.h>


#define PORT_READY  GPIOB
#define PIN_READY   GPIO_PIN_15
#define READY       PORT_READY, PIN_READY

#define PORT_DATA   GPIOB
#define PIN_DATA    GPIO_PIN_14
#define DATA        PORT_DATA, PIN_DATA

#define PORT_CS     GPIOD
#define PIN_CS      GPIO_PIN_8
#define CS          PORT_CS, PIN_CS

#define PORT_WR     GPIOD
#define PIN_WR      GPIO_PIN_5
#define WR          PORT_WR, PIN_WR

#define PORT_RD     GPIOD
#define PIN_RD      GPIO_PIN_4
#define RD          PORT_RD, PIN_RD

#define CONFIG_TO_READ                                                              \
        /*              15,14    1,0   Пишем 00 в биты портов 15, 14, 1, 0      */  \
        GPIOD->MODER &= 0x0ffffff0;                                                 \
        /* Пишем 00 в биты портов 10, 9, 8, 7                                   */  \
        GPIOE->MODER &= 0xffc03fff;

#define CONFIG_TO_WRITE                                                             \
        GPIOD->MODER |= 0x50000005;                                                 \
        GPIOE->MODER |= 0x00154000;


namespace HAL_BUS
{
    struct OutPin
    {
        OutPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

        void Init()
        {
            GPIO_InitTypeDef is = { pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP };

            HAL_GPIO_Init(gpio, &is);

            SetPassive();
        }

        void SetActive() { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET); }

        void SetPassive() { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET); }

        GPIO_TypeDef *gpio;
        uint16 pin;
    };


    struct InPin
    {
        InPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

        void Init()
        {
            GPIO_InitTypeDef is = { pin, GPIO_MODE_INPUT, GPIO_PULLUP };

            HAL_GPIO_Init(gpio, &is);
        }

        bool IsActive() { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

        bool IsPassive() { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

        void WaitPassive() { while (IsActive()) {} }

        void WaitActive() { while (IsPassive()) {} }

        GPIO_TypeDef *gpio;
        uint16 pin;
    };


    // На этом выводе будем выставлять признак готовности к коммуникации и признак подтверждения
    static OutPin pinReady(READY);
    // Здесь будем выставлять признак готовности данных для передачи в устройство
    static OutPin pinData(DATA);
    // По этому сигналу от основого МК начинаем транзакцию чтения/записи
    static InPin  pinCS(CS);
    // Признак того, что основной МК осуществляет операцию записи в панель
    static InPin  pinWR(WR);
    // Признак того, что основной МК осуществляет операцию чтения из панели
    static InPin  pinRD(RD);

    static Queue<uint8> queueData;


    namespace DataBus
    {
        // Первоначальная инициализация
        static void Init();

        static uint8 ReadValue();

        static void WriteValue(uint8);
    };
}


void HAL_BUS::Init()
{
    pinReady.Init();
    pinReady.SetActive();

    pinData.Init();
    pinData.SetPassive();

    pinCS.Init();
    pinRD.Init();
    pinWR.Init();

    DataBus::Init();
}


void HAL_BUS::SendToDevice(uint8 *data, uint size)
{
    while(size > 0)
    {
        queueData.Push(*data++);
        size--;
    }

    if(queueData.Size())
    {
        pinData.SetActive();
    }
}


uint8 HAL_BUS::DataBus::ReadValue()
{
    uint16 port_d = (uint16)GPIOD->IDR;
    uint16 port_e = (uint16)GPIOE->IDR;

    uint8 result = (uint8)((port_d >> 14) & 3);     // добавили биты 0,1
    result |= (port_d << 2) & 0x0C;                 // добавили биты 2,3
    result |= (port_e >> 3) & 0xF0;                 // добавили биты 4,5,6,7

    return result;
}


void HAL_BUS::DataBus::WriteValue(uint8 value)
{

}


void HAL_BUS::Update()
{
    //while(pinCS.IsActive())
    while((GPIOD->IDR & GPIO_PIN_8) == 0)
    {
        //if(pinWR.IsActive())                            // Чтение байта из устройства
        if((GPIOD->IDR & GPIO_PIN_5) == 0)
        {
            //uint8 data = DataBus::Read();
            uint8 data = DataBus::ReadValue();

            //pinReady.SetPassive();
            GPIOB->BSRR = GPIO_PIN_15;

            PDecoder::AddData(data);        // \todo Сейчас недостаток - пока не отработает PDecoder::AddData(), устройство не пойдёт дальше

            //while(pinCS.IsActive()) {};
            //while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) { }
            volatile uint state = GPIOD->IDR & GPIO_PIN_8;
            while(state == 0)
            {
                state = GPIOD->IDR & GPIO_PIN_8;
            }

            //pinReady.SetActive();
            GPIOB->BSRR = (uint)GPIO_PIN_15 << 16U;
        }
        else if(pinRD.IsActive() && queueData.Size())   // Запись байта в устройсто //-V2570 //-V2516
        {
            CONFIG_TO_WRITE;

            // Устанавливаем данные на ШД
            HAL_BUS::DataBus::WriteValue(queueData.Front());

            pinReady.SetPassive();

            pinCS.WaitPassive();

            pinReady.SetActive();

            if(queueData.Size() == 0)
            {
                pinData.SetPassive();
            }

            CONFIG_TO_READ;
        }
    }
}


void HAL_BUS::DataBus::Init()
{
    // Конфигурируем ШД на чтение
    CONFIG_TO_READ;
}
