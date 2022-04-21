// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include <stm32f4xx_hal.h>


#define PORT_READY  GPIOC
#define PIN_READY   GPIO_PIN_14
#define READY       PORT_READY, PIN_READY

#define PORT_DATA   GPIOC
#define PIN_DATA    GPIO_PIN_15
#define DATA        PORT_DATA, PIN_DATA

#define PORT_CS     GPIOC
#define PIN_CS      GPIO_PIN_13
#define CS          PORT_CS, PIN_CS

#define PORT_WR     GPIOD
#define PIN_WR      GPIO_PIN_5
#define WR          PORT_WR, PIN_WR

#define PORT_RD     GPIOD
#define PIN_RD      GPIO_PIN_4
#define RD          PORT_RD, PIN_RD


struct OutPin
{
    OutPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);

        SetPassive();
    }

    void SetActive()  { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET); }

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

    bool IsActive()    { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

    bool IsPassive()   { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

    void WaitPassive() { while(IsActive()) { } }

    void WaitActive()  { while(IsPassive()) { } }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


// �� ���� ������ ����� ���������� ������� ���������� � ������������ � ������� �������������
static OutPin pinReady(READY);
// ����� ����� ���������� ������� ���������� ������ ��� �������� � ����������
static OutPin pinData(DATA);
// �� ����� ������� �� �������� �� �������� ���������� ������/������
static InPin  pinCS(CS);
// ������� ����, ��� �������� �� ������������ �������� ������ � ������
static InPin  pinWR(WR);
// ������� ����, ��� �������� �� ������������ �������� ������ �� ������
static InPin  pinRD(RD);

static Queue<uint8> queueData;


struct DataBus
{
    // �������������� �������������
    static void Init();
};


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


void HAL_BUS::Update()
{
    //while(pinCS.IsActive())
    while((GPIOC->IDR & GPIO_PIN_13) == 0)
    {
        //if(pinWR.IsActive())                            // ������ ����� �� ����������
        if((GPIOD->IDR & GPIO_PIN_5) == 0)
        {
            //uint8 data = DataBus::Read();
            uint8 data = (uint8)GPIOE->IDR;

            //pinReady.SetPassive();
            GPIOC->BSRR = GPIO_PIN_14;

            PDecoder::AddData(data);        // \todo ������ ���������� - ���� �� ���������� PDecoder::AddData(), ���������� �� ����� ������

            //while(pinCS.IsActive()) {};
            //while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) { }
            volatile uint state = GPIOC->IDR & GPIO_PIN_13;
            while(state == 0)
            {
                state = GPIOC->IDR & GPIO_PIN_13;
            }

            //pinReady.SetActive();
            GPIOC->BSRR = (uint)GPIO_PIN_14 << 16U;
        }
        else if(pinRD.IsActive() && queueData.Size())   // ������ ����� � ��������� //-V2570 //-V2516
        {
            // ������������� �� �� ������
            GPIOE->MODER &= 0xffff0000U;
            GPIOE->MODER |= 0x00005555U;

            // ������������� ������ �� ��
            GPIOE->ODR = (GPIOD->ODR & 0xffff0000U) + static_cast<uint16>(queueData.Front());

            pinReady.SetPassive();

            pinCS.WaitPassive();

            pinReady.SetActive();

            if(queueData.Size() == 0)
            {
                pinData.SetPassive();
            }

            // ������������� �� �� ������
            GPIOE->MODER &= 0xffff0000U;
        }
    }
}


void DataBus::Init()
{
    // ������������� �� �� ������
    GPIOE->MODER &= 0xffff0000U;
}
