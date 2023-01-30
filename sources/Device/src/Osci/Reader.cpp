// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Settings/Settings.h"
#include <cstring>


// ��� ������������ CCM-������
static uint8 ccm[8][8 * 1024] __attribute__((section("CCM_DATA")));

// ��� ��������� �� ��������� ������
uint8 *dataIN[2] = { ccm[0], ccm[1] };

// ��� ��������� �� ������, ������������� � ������� � ������
uint8 *dataOUT[2] = { ccm[2], ccm[3] };

// 
uint16 *ave[2] = { IntRAM::Averager32k(ChA), IntRAM::Averager32k(ChB) };

DataSettings ds;

// ��������� �� ��������� ��������� ������
const DataSettings *pDS = nullptr;


FrameP2P *frameP2P = nullptr;


void Reader::ReadDataFromRAM()
{
    DS = nullptr;

    DS = RAM::Get(S_MEM_MODE_WORK_IS_RAM ? (int)(RAM::currentSignal) : 0);

    if(Roller::NeedDraw())
    {
        DS = Roller::GetDS();
    }

    if (DS)
    {
        if (DS->en_a)
        {
            std::memcpy(IN_A, DS->ch_a, 8 * 1024);
        }
        if (DS->en_b)
        {
            std::memcpy(IN_B, DS->ch_b, 8 * 1024);
        }

        ds = *DS;
        DS = &ds;
    }
}


void Reader::ReadDataFromROM()
{
    DS = nullptr;

    DS = ROM::Data::Read(NUM_ROM_SIGNAL);

    if (DS)
    {
        if (DS->en_a)
        {
            std::memcpy(IN_A, DS->ch_a, 8 * 1024);
        }
        if (DS->en_b)
        {
            std::memcpy(IN_B, DS->ch_b, 8 * 1024);
        }
    }
}


//void Reader::FindTrigLevelIfNeed()
//{
//    if (S_TRIG_MODE_FIND_IS_AUTO)
//    {
//        TrigLevel::Find();
//    }
//}
