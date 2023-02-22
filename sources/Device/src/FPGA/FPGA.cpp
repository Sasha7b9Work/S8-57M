// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include <cstring>


namespace FPGA
{
    bool   forcedStart = false;
    uint16 post = (uint16)(~(512));
    uint16 pred = (uint16)(~(512));

    bool   isRunning = false;
}


void FPGA::ForcedStart()
{
    uint8 value = (uint8)(S_TRIG_POLARITY_U8 % 2);

    uint8 stop = 0;
    if (Device::InModeRecorder())           // ¬ режиме регистратора
    {
        stop = (1 << BIT_TRIG_ENABLED);     // устанавливаем признак того, что процесс чтени€ данных бесконечен
    }

    HAL_BUS::FPGA::Write8(WR::TRIG, (uint8)(value++ | stop));
    HAL_BUS::FPGA::Write8(WR::TRIG, (uint8)((value % 2) | stop));

    forcedStart = true;
}


uint16 Osci::ReadLastRecord(Ch::E ch)
{
    static uint16 address = 0;

    if(ch == ChA)
    {
        address = (uint16)(HAL_BUS::FPGA::Read(RD::LAST_RECORD_LO) + ((HAL_BUS::FPGA::Read(RD::LAST_RECORD_HI)) << 8));

        static const uint16 deltas[TBase::Count] = { 5, 5, 5, 5, 5, 10, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        if (S_TIME_BASE <= TBase::_500ns)
        {
            address += deltas[S_TIME_BASE];
        }
    }

    return address;
}


void FPGA::Reset()
{
    TShift::Load();

    LoadRegUPR();
}


bool FPGA::IsRunning()
{
    return isRunning;
}


void FPGA::GiveStart(uint16 pr, uint16 po)
{
    HAL_BUS::FPGA::Write16(WR::PRED_LO, pr);
    HAL_BUS::FPGA::Write16(WR::POST_LO, po);
    HAL_BUS::FPGA::Write8(WR::START, 0xff);
}
