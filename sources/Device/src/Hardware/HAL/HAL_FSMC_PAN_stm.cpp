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

    void Init() { HAL_PIO::Init(port, pin, HMode::Output_PP, HPull::Up); SetPassive(); }
    void SetActive() { HAL_PIO::Reset(port, pin); }
    void SetPassive() { HAL_PIO::Set(port, pin); }

    HPort::E port;
    uint16 pin;
};


struct InPin
{
    InPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init() { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    bool IsActive() { return HAL_PIO::Read(port, pin) == 0; };
    bool IsPassive() { return HAL_PIO::Read(port, pin) == 1; };
    void WaitActive() { while (IsPassive()) {} }
    void WaitPassive() { while (IsActive()) {} }

    HPort::E port;
    uint16 pin;
};


struct DataBus
{
    // �������������� �������������
    static void Init();
};


static bool interactionWithPanel = false;       // true ��������, ��� ���� ��������� � �������� ������ � ������� � ������ �� ������� FSMC � ������� � ������ ���������
static bool otherActionsAllowed = true;         // true ��������, ��� �� ����� �����/�������� ��������� � ������ ��������, ����������� � �������� ���������� - ������
// ����� � ���������� ������ � ������������

static OutPin out_CS(PIN_CS);                   // �� ��������� ��������� ����� ���� ������ ����������, ��� ������� �� ����� � ��������������
static OutPin out_WR(PIN_WR);                   // �� ������������ ��������� ����� ���� ������ ����������, ��� ������� �� ����� ������ ��������
static OutPin out_RD(PIN_RD);                   // �� ��������� ��������� ����� ���� ������ ����������, ��� ������� �� ����� ������ ������

static InPin in_ready(PIN_PAN_READY);           // �������� ���������� ����� ���� ������ �������� � ���������� � ��������������
static InPin in_data(PIN_PAN_DATA);             // �������� ��������� ����� ���� �������� � ���, ��� ������ ����� ������ ��� ��������


namespace HAL_BUS
{
    void InitPanel();
}


void HAL_BUS::InitPanel()
{
    in_ready.Init();
    in_data.Init();
    out_CS.Init();

    DataBus::Init();
}


bool HAL_BUS::Panel::Receive()
{
    //if(in_ready.IsPassive() || in_data.IsPassive())
    //if((GPIOA->IDR & GPIO_PIN_7) != GPIO_PIN_RESET || (GPIOC->IDR & GPIO_PIN_4) != GPIO_PIN_RESET)
    if ((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        return false;
    }

    interactionWithPanel = true;

    if (mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        out_RD.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    }

    out_RD.SetActive();

    out_CS.SetActive();

    in_ready.WaitPassive();

    while (in_ready.IsActive()) {};

    uint8 data = 0;

    while (in_ready.IsActive())
    {
        if (in_data.IsPassive())
        {
            goto exit;
        }
    }

    //                                                 4,5,6,7              2,3                          0,1
    data = (uint8)((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14));

    DDecoder::AddData(data);

exit:

    out_RD.SetPassive();

    out_CS.SetPassive();

    interactionWithPanel = false;

    ///
    // \todo ���� ������ ������ ��� ������, �� ����� ������ ��������, ������ ��� ��� ����� �������� ��������
    // 

    if (otherActionsAllowed)
    {
        if (Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if (OSCI_IN_MODE_P2P)
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


namespace HAL_BUS
{
    namespace Panel
    {
        static int num_bytes = 0;
        static uint num_ticks = 0;

        void BeginScene()
        {
            num_bytes = 0;
            num_ticks = 0;
        }

        uint ElapsedUS()
        {
            return num_ticks / 60;
        }

        int TransBytes()
        {
            return num_bytes;
        }
    }
}


void SBuffer::Send() const
{
    HAL_BUS::Panel::Send(&buffer[0], pointer);
}


void HAL_BUS::Panel::SendByte(uint8 byte)
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
    num_bytes += size;

    uint start = TIME_TICKS;

    if (!(GPIOA->IDR & GPIO_PIN_7) && !(GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        while (Receive()) {}
    }

    interactionWithPanel = true;

    if (mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        out_WR.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
    }

    for (int i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             ���� 0,1                                 ���� 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + (uint16)(((int16)d & 0x03) << 14) + (((uint16)(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
        //                                                                          ���� 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + (uint16)(((int16)d & 0xf0) << 3);

        //out_WR.SetActive();                  // ��� ������ ������
        GPIOD->BSRR = (uint)GPIO_PIN_5 << 16U;

        //while(in_ready.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������
        //while(HAL_PIO::Read(HPort::_A, HPin::_7) == 1) { };
        volatile uint state = GPIOA->IDR & GPIO_PIN_7;
        while (state)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //out_CS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������
        GPIOG->BSRR = (uint)GPIO_PIN_12 << 16U;

        //while(in_ready.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��
        state = GPIOA->IDR & GPIO_PIN_7;
        while (state == 0)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //out_WR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
        GPIOD->BSRR = GPIO_PIN_5;

        //out_CS.SetPassive();                 // /
        GPIOG->BSRR = GPIO_PIN_12;
    }

    interactionWithPanel = false;

    if (otherActionsAllowed)
    {
        if (Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if (OSCI_IN_MODE_P2P)
        {
            Roller::ReadPoint();
        }

        if (funcAfterInteraction)
        {
            funcAfterInteraction();
            funcAfterInteraction = nullptr;
        }
    }

    num_ticks += TIME_TICKS - start;
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
