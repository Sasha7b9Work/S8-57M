// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cstring>


namespace Roller
{
    // ������� ���������� ��������� �����
    static void (*addPoint)(BitSet16, BitSet16);
    static void AddPointPeakDetEnabled(BitSet16 dataA, BitSet16 dataB);
    static void AddPointPeakDetDisabled(BitSet16 dataA, BitSet16 dataB);

    // ���������� true, ���� ������ ����� ��������� �������
    static bool FirstDrawThisFrame();

    // ��������� �� ��������� ������� ������
    static DataSettings *ds = nullptr;

    // ������� �����, ������� ����� �������� ���������
    static int currentPoint = 0;

    // � ���� ����� ������� �������� ��������� �������� ����������� ������. ���� firstOnDisplay == -1, �� ����� ��������� ������� ����� � �������� ������ ���������
    static int firstOnDisplay = -1;
}


void Roller::Event::OnStart()
{
    if(!OSCI_IN_MODE_P2P)
    {
        return;
    }

    ds = IntRAM::PrepareForP2P();
    currentPoint = 0;
    firstOnDisplay = -1;

    addPoint = PEAKDET_ENABLED(ds) ? AddPointPeakDetEnabled : AddPointPeakDetDisabled;
}


bool Roller::NeedDraw()
{
    if (!OSCI_IN_MODE_P2P)
    {
        return false;
    }

    if (RAM::NumberDatas() == 0)
    {
        return true;
    }

    DataSettings *last = RAM::Get(0);

    if (!last->IsEquals(*ds))
    {
        return true;
    }

    if (S_TRIG_START_MODE_IS_AUTO)
    {
        return (TIME_MS - last->timeMS > 1000);
    }
    else
    {
        return !last->IsEquals(*ds);
    }
}


void Roller::ReadPoint()
{
    if (FPGA::IsRunning() && (HAL_PIO::Read(PIN_P2P) != 0))
    {
        uint8 flag = HAL_BUS::FPGA::Read(RD::FLAG_LO);

        if ((flag & 1) == 0)
        {
            HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
            uint8 a1 = HAL_BUS::FPGA::ReadA1();
            uint8 a0 = HAL_BUS::FPGA::ReadA0();

            HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
            uint8 b1 = HAL_BUS::FPGA::ReadA1();
            uint8 b0 = HAL_BUS::FPGA::ReadA0();

            if (S_PEAK_DET_ENABLED)
            {
                static uint8 prev_a = 127;
                static uint8 prev_b = 128;

                BitSet16 dataA(prev_a, a1);
                BitSet16 dataB(prev_b, b1);

                addPoint(dataA, dataB);

                prev_a = a0;
                prev_b = b0;
            }
            else
            {
                BitSet16 dataA(a1, a0);
                BitSet16 dataB(b1, b0);

                addPoint(dataA, dataB);
            }
        }
    }
}


void Roller::AddPointPeakDetEnabled(BitSet16 dataA, BitSet16 dataB)
{
    if(ds->ch_a)
    {
        ds->ch_a[currentPoint * 2] = dataA.byte0;
        ds->ch_a[currentPoint * 2 + 1] = dataA.byte1;
    }

    if(ds->ch_b)
    {
        ds->ch_b[currentPoint * 2] = dataB.byte0;
        ds->ch_b[currentPoint * 2 + 1] = dataB.byte1;
    }

    Math::CircleIncrease<int>(&currentPoint, 0, ds->PointsInChannel());
}


void Roller::AddPointPeakDetDisabled(BitSet16 dataA, BitSet16 dataB)
{
    if(ds->ch_a)
    {
        ds->ch_a[currentPoint] = dataA.byte0;
    }

    if(ds->ch_b)
    {
        ds->ch_b[currentPoint] = dataB.byte1;
    }

    Math::CircleIncrease<int>(&currentPoint, 0, ds->PointsInChannel());
}


DataSettings *Roller::GetDS()
{
    return ds;
}


bool Roller::FirstDrawThisFrame()
{
    return firstOnDisplay == (uint)(-1);
}


int Roller::FillScreenBuffer(Ch::E ch, Buffer &buffer, int width)
{
    int numBytes = currentPoint;

    if(PEAKDET_ENABLED(ds))
    {
        width *= 2;
        numBytes *= 2;
    }

    if(FirstDrawThisFrame())
    {
        firstOnDisplay = currentPoint;
        if(PEAKDET_ENABLED(ds))
        {
            firstOnDisplay *= 2;
        }
    }

    buffer.Realloc(width);
    std::memset(buffer.data, VALUE::NONE, (uint)(width));

    uint position = 0;
    uint8 *in = ds->Data(ch);
    uint8 *out = buffer.data;

    for(int i = firstOnDisplay; i < numBytes; i++)
    {
        out[position] = in[i];
        Math::CircleIncrease<uint>(&position, 0, (uint)(width - 1));
    }

    return (int)(PEAKDET_ENABLED(ds) ? (position / 2 - 1) : (position - 1));
}
