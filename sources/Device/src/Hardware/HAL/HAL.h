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
#define HAL_IWDG_REFRESH()  (*(reinterpret_cast<uint *>(0x40003000U)) = IWDG_KEY_RELOAD)            // �������� � #define ��� ���������� ��������
#endif


struct HAL
{
    static void Init();

    static void ErrorHandler(pCHAR, int);

    static void Delay(uint timeMS);

    static uint TimeMS();
};


struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};


// ������������ ��� ������ � ��������
struct HAL_ADC1
{
    static void Init();

    // ������ ��� �������
    static uint ValueBattery();

    // ������ ��� ��������� ����������
    static uint ValueCharger();
};


// ����������� � ������ �������������
struct HAL_ADC3
{
    static void Init();
    static uint16 ValueRandomizer();
};


struct HAL_CRC32
{
    static void Init();

    // ����������� ����������� ����� ������������������ ���� �� buffer. ���� size �� ������ ������, ��������� ����� �� ��������� � ���������� ����������� �����
    static uint Calculate8bit(uint8 *buffer, int size);
};


struct HAL_DAC1
{
    static void Init();
    static void StartDMA(void *points, uint numPoints);
    static void StopDMA();
    static void ConfigTIM7(uint16 prescaler, uint16 period);
};


struct HAL_DAC2
{
    static void Init();
    static void SetValue(uint value);
};


struct HAL_BUS
{
    static void Init();
    // ��������������� ��� ������ �� ���� FSMC � �������� � �������
    static void ConfigureToFSMC();

    struct Panel
    {
        static void Send(uint8 byte);
        static void Send(uint8 byte0, uint8 byte1);
        static void Send(const uint8 *data, int size);
        static bool Receive();
        static bool InInteraction();
        static void RunAfterInteraction(void (*func)());

        // ��������� ��� �������� ����� ��������� (�.�. ������ � ���������� ������ � ������������)
        static void ProhibitOtherActions();

        // ��������� ��������� ������ �������� ����� ��������� (������ � ���������� ������ � ������������)
        static void AllowOtherActions();
    };

    struct FPGA
    {
        static void Write8(uint8 *address, uint8 value);
        static void Write16(uint8 *address, uint16 value);
        static uint8 Read(const uint8 *address);
        // ���������� ����� ��� ������ ������
        static void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
        // ������ ������ � �������������� ������
        static uint8 ReadA0();
        static uint8 ReadA1();
        // ���������� �������� ��� ������� ������
        static float GetStretch(const uint8 *address);
    private:
        // ������ ����� ������ ������
        static uint8 *addrData0;
        // ������ ����� ������ ������
        static uint8 *addrData1;
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

    static Mode::E mode;

private:
    // ��������� FSMC ��� ������ � ������� RAM
    static void InitRAM();
    // ������������� ����� ������, ������� �� �������� ���� ����� �� ����� ���� ������ ���������
    static void InitPanel();
};


struct HAL_HCD
{
    static void Init();

    static void InitUSBH_LL(USBH_HandleTypeDef *phost);

    static void *handleHCD;
};


struct HAL_NVIC
{
    static uint irqEXTI1;
    static uint irqEXTI9_5;
    static void EnableIRQ(uint irq);
    static void DisableIRQ(uint irq);
    static void SetPriorityIRQ(uint irq, uint main, uint sub);
    static void SystemReset();
};


struct HAL_OTP
{
    // ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    static int GetSerialNumber(char buffer[17]);
    static bool SaveSerialNumber(char *servialNumber);
};


struct HAL_PCD
{
    static void Init();
};


struct HAL_ROM
{
    static const Sector sectors[Sector::Count];

    static void WriteBytes(uint address, const uint8 *data, int size);
    static void WriteBufferBytes(uint address, const void *data, int size);
    static void Fill(uint address, uint8 value, int size);
};


struct HAL_SPI4
{
    static void Init();
};


struct HAL_TIM2
{
    static uint TimeUS();
    static uint TimeTicks();
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void StartMultiMeasurement();
};


struct HAL_TIM3
{
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void EnableIRQ(uint mainPriority, uint subPriority);
    static void DisableIRQ();
    static void StartIT(uint period);
    static void StopIT();
};


// ������������ ��� ���������� ������� � ������ ���������
struct HAL_TIM5
{
    static void Init();
    static void ElapsedCallback();
};


struct HAL_USART3
{
    static void Init(pFuncVV recvCallback);
    static void Transmit(void *buffer, int size, uint timeout);
    static void StartReceiveIT(void *buffer, int size);
};
