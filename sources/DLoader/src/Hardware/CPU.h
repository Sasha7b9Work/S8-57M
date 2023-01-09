#pragma once
#include "defines.h"
#include "Hardware/stm32/stm437.h"
#include "Hardware/Controls.h"

#include <usbh_core.h>
#include <stm32f4xx_hal_hcd.h>

#define PRIORITY_SYS_TICK           0, 0
#define PRIORITY_FPGA_ADC           1, 0
#define PRIORITY_FPGA_P2P           1, 1
#define PRIORITY_TIMER_TIM6         2, 0
#define PRIORITY_PANEL_EXTI9_5      3, 0
#define PRIORITY_PANEL_SPI1         4, 0
#define PRIORITY_SOUND_DMA1_STREAM5 5, 0
#define PRIORITY_FLASHDRIVE_OTG     6, 0
#define PRIORITY_VCP_OTG            7, 0
#define PRIORITY_RAM_DMA2_STREAM0   8, 0
    


#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // ����� 8-������ ��������������� �������������� �����



class CPU : public STM437
{
public:
    static void Init();

    static void DeInit();

    
    class Panel
    {
    friend class CPU;
        
    public:

        static void Update();
        
        // �������� ������� � �� ������ ����������.
        static void TransmitData(uint16 data);
        
        // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
        static void Disable();

        static void Enable();
        
        // ������� ������� �������.
        static PanelButton WaitPressingButton();

        static bool ProcessingCommandFromPIC(uint16 command);

        static uint16 NextData();

        static PanelButton PressedButton();

        static void SPI_IRQHandler();
    };


    class FLASH_
    {
    public:
        static const uint ADDR_SECTOR_PROGRAM_0 = ((uint)(0x08020000));   // ����� ������� ������� � ����������

        // ������� �������, ��������������� ��� ������ ��������.
        static void Prepare();
        
        // �������� �����, �� ������� ��������� ��������� data, �������� size, �� ������ address.
        static void WriteData(uint address, uint8 *data, int size);
    };

private:
    static void EnablePeriphery();

    static void InitHardware();
};



#ifdef __cplusplus
extern "C" {
#endif

    // ���������� �� ������
    void TIM3_IRQHandler();
    void EXTI9_5_IRQHandler();
    void SPI1_IRQHandler();
    void OTG_FS_IRQHandler();

    void NMI_Handler();
    void HardFault_Handler();
    void MemManage_Handler();
    void BusFault_Handler();
    void UsageFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void PendSV_Handler();
    void SysTick_Handler();

#ifdef __cplusplus
}
#endif
