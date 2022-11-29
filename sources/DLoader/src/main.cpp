/*
    ��������� ������������� �� ������ 0x08000000
    ����� ������ �� ��������� ������� ������.
        ���� ������ ����:
            ��������� ������� ����� S8-54.bin. ���� ������� �������, ������� ��������� "�� USB-����� ���������� ������ ����������� �����������. ����������?"
            ���� ������ "��":
                1. ��������� �������:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. �� �� ����� ������������ ���������� ����� S8-54.bin
        ���� ������ �������������� �� �������:
            ������� ��������� "�� ������� �������������� ������. ���������, ��� �� ��� �������� ������� fat32"
    ����� ����������� ������� �� ������, ���������� � 0x0802004 (������ 32�-������ ����� � ������� ��������, ������������� �� ������ 0x0802000)
*/

#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"


#define MAIN_PROGRAM_START_ADDRESS  0x8020000


typedef void(*pFunction)();


int main()
{
    CPU::Init();

    Timer::PauseOnTime(250);
    
    Display::Init();
   
    //Timer::SetAndEnable(kTemp, Display::Update, 10);

    FDrive::Init();

    FDrive::AttemptUpdate();
    
    Timer::Disable(kTemp);

    //while (Display::IsRun())
    //{
    //}
    
    CPU::DeInit();

    __disable_irq();
    // ������ ��������� �� �������� ���������
    pFunction JumpToApplication;
    JumpToApplication = reinterpret_cast<pFunction>(*reinterpret_cast<__IO uint *>(MAIN_PROGRAM_START_ADDRESS + 4)); //-V2571
    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();

    return 0;
}
