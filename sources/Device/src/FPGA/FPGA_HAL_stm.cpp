// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "globals.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


static uint timeFireTrig = 0;   // ����� ��������� �������� �������������

extern uint16 flagFPGA;

void FPGA::Flag::Read(bool updateFreqMeter)
{
    flagFPGA = static_cast<uint16>(HAL_BUS::FPGA::Read(RD::FLAG_LO) | (HAL_BUS::FPGA::Read(RD::FLAG_HI) << 8));

    if (Flag::TrigReady() && !forcedStart)
    {
        timeFireTrig = TIME_MS;
        Trig::pulse = true;
        OsciStateWork::triggered = true;
    }

    if(!Flag::TrigReady() && TIME_MS - timeFireTrig > 1000)
    {
        Trig::pulse = false;
    }

    if (updateFreqMeter)
    {
        FreqMeter::Update();
    }
}
