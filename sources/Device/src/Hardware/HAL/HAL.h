// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Memory/Sector.h"
#include "Osci/DeviceSettings.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()             ::HAL::ErrorHandler(__FILE__, __LINE__)


#define HAL_BUS_CONFIGURE_TO_FSMC() if(HAL_BUS::mode != HAL_BUS::Mode::FSMC) { HAL_BUS::ConfigureToFSMC(); }


typedef struct _USBH_HandleTypeDef USBH_HandleTypeDef;


#ifdef WIN32
#define HAL_IWDG_REFRESH()
#else
#define HAL_IWDG_REFRESH()  (*((uint *>(0x40003000U)) = IWDG_KEY_RELOAD)            // �������� � #define ��� ���������� ��������
#endif


namespace HAL
{
    void Init();

    void ErrorHandler(pchar, int);

    void Delay(uint timeMS);

    uint TimeMS();
};


namespace HAL_RTC
{
    void Init();
    PackedTime GetPackedTime();
    bool SetPackedTime(const PackedTime &time);
};


// ������������ ��� ������ � ��������
namespace HAL_ADC1
{
    void Init();

    // ������ ��� �������
    uint ValueBattery();

    // ������ ��� ��������� ����������
    uint ValueCharger();
};


// ����������� � ������ �������������
namespace HAL_ADC3
{
    void Init();
    uint16 ValueRandomizer();
};


namespace HAL_CRC32
{
    void Init();

    // ����������� ����������� ����� ������������������ ���� �� buffer. ���� size �� ������ ������, ��������� ����� �� ��������� � ���������� ����������� �����
    uint Calculate8bit(uint8 *buffer, int size);
};


namespace HAL_DAC1
{
    void Init();
    void StartDMA(void *points, uint numPoints);
    void StopDMA();
    void ConfigTIM7(uint16 prescaler, uint16 period);
};


namespace HAL_BUS
{
    void Init();

    // ��������������� ��� ������ �� ���� FSMC � �������� � �������
    void ConfigureToFSMC();

    namespace Panel
    {
        void BeginScene();
        int TransBytes();
        uint ElapsedUS();

        void SendByte(uint8 byte);
        void SendBuffer(const uint8 *data, int size);
        bool Receive();
        bool InInteraction();
        void RunAfterInteraction(void (*func)());

        // ��������� ��� �������� ����� ��������� (�.�. ������ � ���������� ������ � ������������)
        void ProhibitOtherActions();

        // ��������� ��������� ������ �������� ����� ��������� (������ � ���������� ������ � ������������)
        void AllowOtherActions();
    };

    namespace FPGA
    {
        void Write8(uint8 *address, uint8 value);
        void Write16(uint8 *address, uint16 value);
        uint8 Read(const uint8 *address);
        // ���������� ����� ��� ������ ������
        void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
        // ������ ������ � �������������� ������
        uint8 ReadA0();
        uint8 ReadA1();
        // ���������� �������� ��� ������� ������
        float GetStretch(const uint8 *address);
    };

    struct Mode
    {
        enum E
        {
            FSMC,
            PanelRead,
            PanelWrite
        };
    };

    extern Mode::E mode;
};


namespace HAL_HCD
{
    void Init();

    void InitUSBH_LL(USBH_HandleTypeDef *phost);

    extern void *handleHCD;
};


namespace HAL_NVIC
{
    extern uint irqEXTI1;
    extern uint irqEXTI9_5;
    void EnableIRQ(uint irq);
    void DisableIRQ(uint irq);
    void SetPriorityIRQ(uint irq, uint main, uint sub);
    void SystemReset();
};


namespace HAL_OTP
{
    // ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    int GetSerialNumber(char buffer[17]);
    bool SaveSerialNumber(char *servialNumber);
};


namespace HAL_PCD
{
    void Init();
};


namespace HAL_ROM
{
    extern const Sector sectors[Sector::Count];

    void WriteBytes(uint address, const uint8 *data, int size);
    void WriteBufferBytes(uint address, const void *data, int size);
    void Fill(uint address, uint8 value, int size);
};


namespace HAL_SPI4
{
    void Init();
};


namespace HAL_TIM2
{
    uint TimeUS();
    uint TimeTicks();
    void Init(uint prescaler, uint period);
    void DeInit();
    void Start();
    void Stop();
    void StartMultiMeasurement();
};


namespace HAL_TIM3
{
    void Init(uint prescaler, uint period);
    void DeInit();
    void Start();
    void Stop();
    void EnableIRQ(uint mainPriority, uint subPriority);
    void DisableIRQ();
    void StartIT(uint period);
    void StopIT();
};


// ������������ ��� ���������� ������� � ������ ���������
namespace HAL_TIM5
{
    void Init();
    void ElapsedCallback();
};


namespace HAL_USART3
{
    void Init(pFuncVV recvCallback);
    void Transmit(void *buffer, int size, uint timeout);
    void StartReceiveIT(void *buffer, int size);
};
