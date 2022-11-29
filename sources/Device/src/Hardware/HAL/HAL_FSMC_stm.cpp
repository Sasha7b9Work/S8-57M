#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include <stm32f4xx_hal.h>


#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
//#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
//#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
//#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)


uint8 *HAL_BUS::FPGA::addrData0 = nullptr;
uint8 *HAL_BUS::FPGA::addrData1 = nullptr;


HAL_BUS::Mode::E HAL_BUS::mode = HAL_BUS::Mode::FSMC;


static GPIO_InitTypeDef is =
{
    GPIO_PIN_0,
    GPIO_MODE_AF_PP,
    GPIO_PULLUP,
    GPIO_SPEED_FREQ_VERY_HIGH,
    GPIO_AF12_FMC
};


void HAL_BUS::Init()
{
    __HAL_RCC_FMC_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOF_CLK_ENABLE(); //-V2571

    //           D2           D3           D0            D1
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &is); //-V2571

    //           D4           D5           D6           D7
    is.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &is); //-V2571

    //           A0           A1           A2           A3           A4           A5           A6            A7            A8            A9
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &is); //-V2571

    //           A10          A11         A12           A13          A14         A15           NE3
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOG, &is); //-V2571

    //           A16          A17            A18
    is.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &is); //-V2571

    static const GPIO_InitTypeDef isGPIO =
    {   //    NOE          NWE          NE1
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    // \todo ����� �� �������� - �� �����, ������-��

    HAL_GPIO_Init(GPIOD, const_cast<GPIO_InitTypeDef *>(&isGPIO)); //-V2567 //-V2571


    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE, //-V2571
        FMC_NORSRAM_EXTENDED_DEVICE, //-V2571
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                           // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    SRAM_HandleTypeDef *hsram = &gSramHandle;
    FMC_NORSRAM_TimingTypeDef *Timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming); //-V2567

    FMC_NORSRAM_Init(hsram->Instance, &(hsram->Init));

    FMC_NORSRAM_Timing_Init(hsram->Instance, Timing, hsram->Init.NSBank);

    hsram->Extended->BWTR[hsram->Init.NSBank] = 0x0FFFFFFFU;

    __FMC_NORSRAM_ENABLE(hsram->Instance, hsram->Init.NSBank);

    InitPanel();

    InitRAM();
}


void HAL_BUS::InitRAM()
{
    __HAL_RCC_GPIOD_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOG_CLK_ENABLE(); //-V2571

    //           A16           A17           A18 
    is.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &is); //-V2571

    //           A10          A11         A12           A13          A14         A15           NE3
    is.Pin =  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOG, &is); //-V2571

    //           A6            A7          A8            A9
    is.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &is); //-V2571

    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE, //-V2571
        FMC_NORSRAM_EXTENDED_DEVICE, //-V2571
        {
            FMC_NORSRAM_BANK3,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                           // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming); //-V2567

    HAL_SRAM_Init(&gSramHandle, timing, timing);
}


void HAL_BUS::ConfigureToFSMC()
{
    mode = Mode::FSMC;

    //           NOE          NWE          NE1
    is.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;   
    HAL_GPIO_Init(GPIOD, &is); //-V525 //-V2571

    //           D2           D3           D0            D1
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &is); //-V2571

    //           D4           D5           D6           D7
    is.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &is); //-V2571
}


void HAL_BUS::FPGA::Write16(uint8 *address, uint16 value)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    PAUSE_ON_TICKS(100);    // \todo ��� ���� ������ �������� ����� ��� ���������� ����������� � TBase >= 0.5��

    *address = static_cast<uint8>(value);
    *(address + 1) = static_cast<uint8>(value >> 8); //-V2563


}


void HAL_BUS::FPGA::Write8(uint8 *address, uint8 value)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    *address = value;
}


uint8 HAL_BUS::FPGA::Read(const uint8 *address)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    return *address;
}


void HAL_BUS::FPGA::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;

    HAL_BUS_CONFIGURE_TO_FSMC();
}


uint8 HAL_BUS::FPGA::ReadA0() //-V2506
{
    int delta = VALUE::AVE - static_cast<int>(*addrData0);

    int result = static_cast<int>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData0))); //-V2564

    if(result < VALUE::MIN)
    {
        return VALUE::MIN;
    }
    else if(result > VALUE::MAX) //-V2516
    {
        return VALUE::MAX;
    }

    return static_cast<uint8>(result);
}


uint8 HAL_BUS::FPGA::ReadA1() //-V2506
{
    int delta = VALUE::AVE - static_cast<int>(*addrData1);

    int result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData1))); //-V2564

    if(result < VALUE::MIN)
    {
        return VALUE::MIN;
    }
    else if(result > VALUE::MAX) //-V2516
    {
        return VALUE::MAX;
    }

    return static_cast<uint8>(result);
}


float HAL_BUS::FPGA::GetStretch(const uint8 *address)
{
    static const float *stretchs[4] =
    {
        &NRST_EX_STRETCH(ChanA),
        &NRST_EX_STRETCH(ChanA),
        &NRST_EX_STRETCH(ChanB),
        &NRST_EX_STRETCH(ChanB)
    };

    int delta = address - RD::DATA_A;

    return *stretchs[delta];
}
