// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>
#ifdef DEVICE
#include "Osci/Osci.h"
#endif


static void (*funcAfterInteraction)() = nullptr;      // ��� ������� ����� ������� ����� ��������� �������������� � �������


struct OutPin
{
    OutPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()       { HAL_PIO::Init(port, pin, HMode::Output_PP, HPull::Up); SetPassive(); }
    void SetActive()  { HAL_PIO::Reset(port, pin); }
    void SetPassive() { HAL_PIO::Set(port, pin); }

    HPort::E port;
    uint16 pin;
};


struct InPin
{
    InPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()        { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    bool IsActive()    { return HAL_PIO::Read(port, pin) == 0; };
    bool IsPassive()   { return HAL_PIO::Read(port, pin) == 1; };
    void WaitActive()  { while(IsPassive()) { } }
    void WaitPassive() { while(IsActive()) { } }

    HPort::E port;
    uint16 pin;
};


struct DataBus
{
    // �������������� �������������
    static void Init();
};


static bool interactionWithPanel = false;       // true ��������, ��� ���� ��������� � �������� ������ � ������� � ������ �� ������� FSMC � ������� � ������ ���������
static bool otherActionsAllowed = true;         // true ��������, ��� �� ����� �����/�������� ��������� � ������ ��������, ����������� � �������� ���������� - ������ ����� � ����������
                                                // ������ � ������������


static OutPin pinD0(HPort::_D, HPin::_14);
static OutPin pinD1(HPort::_D, HPin::_15);
static OutPin pinD2(HPort::_D, HPin::_0);
static OutPin pinD3(HPort::_D, HPin::_1);
static OutPin pinD4(HPort::_E, HPin::_7);
static OutPin pinD5(HPort::_E, HPin::_8);
static OutPin pinD6(HPort::_E, HPin::_9);
static OutPin pinD7(HPort::_E, HPin::_10);

static OutPin pinWR(PIN_WR);
static OutPin pinRD(PIN_RD);
static OutPin pinCS(PIN_CS);
static OutPin pinReady(PIN_PAN_READY);
static OutPin pinData(PIN_PAN_DATA);

void HAL_BUS::InitPanel()
{
    pinD0.Init();
    pinD1.Init();
    pinD2.Init();
    pinD3.Init();
    pinD4.Init();
    pinD5.Init();
    pinD6.Init();
    pinD7.Init();

    pinWR.Init();
    pinRD.Init();
    pinCS.Init();
    pinReady.Init();
    pinData.Init();
}


void HAL_BUS::UpdatePanel()
{
    static OutPin *pins[13] = {   &pinD0, &pinD1, &pinD2, &pinD3, &pinD4, &pinD5, &pinD6, &pinD7,
                                        &pinWR, &pinRD, &pinCS, &pinReady, &pinData};

    static int counter = 0;

    for (int i = 0; i < 13; i++)
    {
        if (i == counter)
        {
            pins[i]->SetActive();
        }
        else
        {
            pins[i]->SetPassive();
        }
    }

    counter++;

    if (counter > 12)
    {
        counter = 0;
    }
}


bool HAL_BUS::Panel::Receive() //-V2506
{
    //if(pinReadyPAN.IsPassive() || pinDataPAN.IsPassive())
    //if((GPIOA->IDR & GPIO_PIN_7) != GPIO_PIN_RESET || (GPIOC->IDR & GPIO_PIN_4) != GPIO_PIN_RESET)
    if((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        return false;
    }
   
    interactionWithPanel = true;
    
    if(mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        pinRD.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    }
    
    pinRD.SetActive();

    pinCS.SetActive();
    
//    pinReadyPAN.WaitPassive();

//    while(pinReadyPAN.IsActive()) {};
    
    uint8 data = 0;

//    while(pinReadyPAN.IsActive())
//    {
//        if(pinDataPAN.IsPassive())
//        {
//            goto exit;
//        }
//    }
    
    //                                                 4,5,6,7              2,3                          0,1
    data = static_cast<uint8>((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14));

    DDecoder::AddData(data);
    
//exit:
    
    pinRD.SetPassive();

    pinCS.SetPassive();
    
    interactionWithPanel = false;

    ///
    // \todo ���� ������ ������ ��� ������, �� ����� ������ ��������, ������ ��� ��� ����� �������� ��������
    // 

    if(otherActionsAllowed)
    {
        if(Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if(OSCI_IN_MODE_P2P)
        {
            Roller::ReadPoint();
        }
        else
        {
            Timer::PauseOnTicks(10);
        }
    }
    else
    {
        Timer::PauseOnTicks(10);
    }

    return true;
}


void HAL_BUS::Panel::Send(uint8 byte)
{
    Send(&byte, 1);
}


void HAL_BUS::Panel::Send(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    Send(buffer, 2);
}

void HAL_BUS::Panel::Send(const uint8 *data, int size)
{
    if(!(GPIOA->IDR & GPIO_PIN_7) && !(GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        while(Receive()) { }
    }

    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        pinWR.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
    }

    for(int i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             ���� 0,1                                 ���� 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
        //                                                                          ���� 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        //pinWR.SetActive();                  // ��� ������ ������
        GPIOD->BSRR = (uint)GPIO_PIN_5 << 16U;
        
        //while(pinReadyPAN.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������
        //while(HAL_PIO::Read(HPort::_A, HPin::_7) == 1) { };
        volatile uint state = GPIOA->IDR & GPIO_PIN_7;
        while(state)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //pinCS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������
        GPIOG->BSRR = (uint)GPIO_PIN_12 << 16U;

        //while(pinReadyPAN.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��
        state = GPIOA->IDR & GPIO_PIN_7;
        while(state == 0)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //pinWR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
        GPIOD->BSRR = GPIO_PIN_5;

        //pinCS.SetPassive();                 // /
        GPIOG->BSRR = GPIO_PIN_12;
    }

    interactionWithPanel = false;

    if(otherActionsAllowed)
    {
        if(Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if(OSCI_IN_MODE_P2P) //-V2516
        {
            Roller::ReadPoint();
        }

        if(funcAfterInteraction)
        {
            funcAfterInteraction();
            funcAfterInteraction = nullptr;
        }
    }
}


bool HAL_BUS::Panel::InInteraction()
{
    return interactionWithPanel;
}


void HAL_BUS::Panel::RunAfterInteraction(void (*func)())
{
    funcAfterInteraction = func;
}


void DataBus::Init()
{
    // ������������� �� �� ������

    GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

    GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
}


void HAL_BUS::Panel::ProhibitOtherActions()
{
    otherActionsAllowed = false;
}


void HAL_BUS::Panel::AllowOtherActions()
{
    otherActionsAllowed = true;
}
