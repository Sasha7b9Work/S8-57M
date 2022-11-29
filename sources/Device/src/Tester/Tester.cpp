#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


// ������� ���
static int step = 0;

// ��� ��������� ����������
static float stepU = 0.0F;

// ������������� � true �������� ��������, ��� ������ ����� �������
static bool enabled = false;

static Settings oldSet = Settings::defaultSettings;

static uint16 dataX[Tester::NUM_STEPS][TESTER_NUM_POINTS];  // \todo ������� ���, ����� ��� ��������� ������-���������� ����������� ������ ������� �� Heap.cpp
static uint8  dataY[Tester::NUM_STEPS][TESTER_NUM_POINTS];

bool Tester::needSended[Tester::NUM_STEPS] = { false, false, false, false, false };
bool Tester::sended[Tester::NUM_STEPS] = { false, false, false, false, false };

// ������ ������ � ����
static void ReadFPGA(uint16 *dataA, uint8 *dataB);

// ��������� ���� ������ ��� ������-����������. � ������� time ������ ������ ���� ������� numPoints �����
// ���� ���������� false - ����� �� ������
static bool StartFPGA();

// ����������� ����� ��� ������� ���������
static void RecountPoints(uint16 *x, uint8 *y);


void Tester::Init()
{
    for (int i = 0; i < Chan::Count; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                dataX[j][k] = 0;
                dataY[j][k] = 0;
            }
        }
    }

    HAL_PIO::Init(PIN_TESTER_DAC, HMode::Analog, HPull::No);
    HAL_PIO::Init(PIN_TESTER_PNP, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_ON, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_I, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_U, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_STR, HMode::RisingIT, HPull::No);

    HAL_PIO::Set(PIN_TESTER_ON);         // ��������� ������-��������

    HAL_DAC2::Init();

    Disable();
}


void Tester::DeInit()
{

}


void Tester::Update()
{
}


void Tester::Enable() // -V2506
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    ModeCouple::Set(ChanA, ModeCouple::GND);
    ModeCouple::Set(ChanB, ModeCouple::GND);

    Range::Set(ChanA, Range::_2V);
    Range::Set(ChanB, Range::_2V);

    RShift::Set(ChanA, 0);
    RShift::Set(ChanB, 0);

    HAL_PIO::Reset(PIN_TESTER_ON);       // �������� ������-���������

    Osci::Stop();

    step = 0;                               // ������ ��������� ������� ����� �������� ������� ���

    LoadPolarity();

    LoadStep();

    HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // �������� ���������� �� �������� ���������
}


int16 Tester::DeltaRShiftA()
{
    return 18;
}


void Tester::Disable() // -V2506
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // ��������� ���������� �� �������� ���������

    HAL_PIO::Set(PIN_TESTER_ON);

    oldSet.test._control = S_TEST_CONTROL;
    oldSet.test._polarity = S_TEST_POLARITY;
    oldSet.test._stepU = S_TEST_STEP_U;
    oldSet.test._stepI = S_TEST_STEP_I;

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}


void Tester::StartStop()
{
    if (enabled)
    {
        HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // ��������� ���������� �� �������� ���������
        enabled = false;
    }
    else
    {
        HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // �������� ���������� �� �������� ���������
        enabled = true;
    }
}


void Tester::ProcessStep() //-V2506
{
                                                                                                                                                /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 ��   | step == 2 |  2.5 ��   | step == 4 |  2.5 ��   | step == 6 |  2.5 ��   | step == 8 |  2.5 ��   |
       |     0V    |  ������   |   1 * dU  |  ������   |   2 * dU  |  ������   |   3 * dU  |  ������   |  4 * dU   |  ������   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                */


    if(HAL_BUS::Panel::InInteraction())
    {
        HAL_BUS::Panel::RunAfterInteraction(Tester::ProcessStep);
        return;
    }

    if (Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // ���� ��� ������ ����, �� ����� ������������� ����������
    {
        HAL_DAC2::SetValue(static_cast<uint>(stepU * step / 2)); //-V2564
        // ��������� ���� ��� ������ ������������ ���������� �����. ����� ����� ������������� � ������� 2.5 �� (������������� ������ �����)
        if(!StartFPGA())
        {
            return;
        }
    }
    else
    {
        static int countRead = 0;               // ������� ������� ������
        countRead++;

        FPGA::Flag::Read();
        if(FPGA::Flag::DataReady())
        {
            ReadData();
            countRead = 0;
        }
        else
        {
            if(countRead > 10)
            {
                step--;
            }
            return;
        }
    }

    ++step;

    if (step == 10)
    {
        step = 0;
    }
}


void Tester::ReadData()
{
    if (SCPI::Sender::tester)
    {
        SCPI::Sender::tester = false;

        for (int i = 0; i < NUM_STEPS; i++)
        {
            sended[i] = false;
            needSended[i] = true;
        }
    }

    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

    ReadFPGA(x, y);

    if (needSended[halfStep] && !sended[halfStep])
    {
        needSended[halfStep] = false;
        sended[halfStep] = true;

        SCPI::SendData(String("STEP %d : ", halfStep).c_str());

        for (int i = 0; i < TESTER_NUM_POINTS; i++)
        {
            SCPI::SendData(String("%d:%d ", x[i], y[i]).c_str()); //-V2563
        }
        SCPI::SendAnswer(" ");
    }

    RecountPoints(x, y);

    DisplayTester::SetPoints(halfStep, x, y); //-V2571
}


static void RecountPoints(uint16 *x, uint8 *y)
{
    static const float scaleX = 332.0F / 240.0F;
    static const float scaleY = 249.0F / 255.0F;

    static const int dX = 20;
    static const int dY = -2;

    static const int x0 = 160;
    static const int y0 = 120;

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int X = 255 - x[i] + dX; //-V2563
        X = static_cast<int>(x0 + (x0 - X) * scaleX); //-V2564
        LIMITATION(X, 0, 319);
        x[i] = static_cast<uint16>(X); //-V2563

        int Y = y[i] + dY; //-V2563
        Y = static_cast<uint8>(y0 + (y0 - Y) * scaleY); //-V2564
        LIMITATION(Y, 0, 239);
        y[i] = static_cast<uint8>(Y); //-V2563
    }
}


void Tester::LoadPolarity()
{
    // ������������� ����������
    HAL_PIO::Write(PIN_TESTER_PNP, S_TEST_POLARITY_IS_POSITIVE ? 1 : 0);
}


// ���������� true, ���� ��� �������� ������ ������ ����� ����������� ���������
static bool NeedSmallStep() //-V2506
{
    if(S_TEST_CONTROL_IS_VOLTAGE)
    {
        return S_TEST_STEP_U_IS_600mV;
    }

    return S_TEST_STEP_I_IS_4uA;
}


void Tester::LoadStep()
{
    // ������������� ���������� ����������� ��� �����
    HAL_PIO::Write(PIN_TESTER_U, S_TEST_CONTROL_IS_VOLTAGE ? 1 : 0);

    HAL_PIO::Write(PIN_TESTER_I, S_TEST_CONTROL_IS_VOLTAGE ? 0 : 1);

    stepU = 255.0F / 5; //-V2564

    if(NeedSmallStep())
    {
        stepU /= 5; //-V2564
    }
}


pString Tester::Scale::ToString() const // -V2506
{
    if (ch == ChanA)
    {
        return Range::ToString(static_cast<Range::E>(value), Divider::_1);
    }

    static const pString names[] =
    {
        "2���",
        "5���",
        "10���",
        "20���",
        "50���",
        "100���",
        "200���",
        "500���",
        "1��",
        "2��",
        "5��",
        "10��",
        "20��"
    };

    pString name = names[value];

    if (name)
    {
        return names[value];
    }

    return 0;
}


String Tester::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == ChanA)
    {
        return RShift::ToString(shift, static_cast<Range::E>(scale), Divider::_1);
    }

    float shiftAbs = RShift::ToAbs(shift, static_cast<Range::E>(scale)) * 1e-3F;

    return Current(shiftAbs).ToString();
}


static void ReadFPGA(uint16 *dataA, uint8 *dataB)
{
    uint16 aRead = (uint16)(Osci::ReadLastRecord(ChanA) - TESTER_NUM_POINTS);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � �������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    HAL_BUS::FPGA::SetAddrData(RD::DATA_A + 1, RD::DATA_B + 1); //-V2563

    for(int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataA++ = HAL_BUS::FPGA::ReadA0();
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // ��������� �����, � ��������� ����� ������ ������
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // � ��� ������� ����, ����� ������ ���������� � ����

    for(int i = 0; i < TESTER_NUM_POINTS; i++)         // ������ ������ ������� ������
    {
        *dataB++ = HAL_BUS::FPGA::ReadA1();
    }
}


static bool StartFPGA() //-V2506
{
    // � ��� ���������� �������. �� ���������� ������� ������ ����������� �� ����� 2.5 ��
    // 2.5�� / 12��� = 0.2 ��/��� = 10���/���

    TBase::Set(TBase::_500us);

    FPGA::GiveStart(static_cast<uint16>(~(1)), static_cast<uint16>(~(TESTER_NUM_POINTS + 1)));

    for(int i = 0; i < 20; i++)
    {
        FPGA::Flag::Read();
        if(FPGA::Flag::Pred())
        {
            FPGA::ForcedStart();
            return true;
        }
    }

    return false;
}
