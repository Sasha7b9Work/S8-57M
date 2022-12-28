// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"


namespace Osci
{
    // Читать данные канала в памяить data
    bool ReadDataChannel(Chan::E ch, uint8 *data);

    bool ReadDataChannelRand(uint8 *address, uint8 *data);

    extern uint16 addrRead;
}


bool Osci::ReadDataChannel(Chan::E ch, uint8 *data)
{
    int numPoints = ENumPointsFPGA::PointsInChannel();

    if(addrRead == 0xffff)
    {
        addrRead = static_cast<uint16>(ReadLastRecord(ch) - (int)(numPoints) / TBase::DeltaPoint() - (OSCI_IN_MODE_RANDOMIZER ? 10 : 12));
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, static_cast<uint16>(addrRead));
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = (ch == ChanA) ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        return ReadDataChannelRand(a1, data);
    }
    else
    {
        uint8 *p = data;

        *p = HAL_BUS::FPGA::ReadA0();    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = HAL_BUS::FPGA::ReadA1();    // -V519

        if(PeakDetMode().IsEnabled())
        {
            for(int i = 0; i < numPoints; i++)
            {
                *p++ = HAL_BUS::FPGA::ReadA0();
                *p++ = HAL_BUS::FPGA::ReadA1();
            }
        }
        else
        {
            float stretch = HAL_BUS::FPGA::GetStretch(a1);

            for(int i = 0; i < numPoints; i++)
            {
                int delta = VALUE::AVE - (int)(*a1);

                int result = (int)(VALUE::AVE - (int)(delta * stretch));

                if(result < VALUE::MIN)
                {
                    p[i] = VALUE::MIN;
                }
                else if(result > VALUE::MAX)
                {
                    p[i] = VALUE::MAX;
                }
                else
                {
                    p[i] = (uint8)(result);
                }
            }
        }
    }

    return true;
}
