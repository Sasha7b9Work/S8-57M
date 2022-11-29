#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


void TrigStartMode::Set(TrigStartMode::E v)
{
    S_TRIG_START_MODE = v;
    Osci::ChangedTrigStartMode();
}


void TrigSource::Set(Chan::E ch)
{
    if (ch != Chan::Count)
    {
        S_TRIG_SOURCE = ch;
    }

    TrigInput::Load();
}


void TrigInput::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // �� 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ��
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // ��
    };

    HAL_PIO::Write(PIN_LF3, _GET_BIT(datas[S_TRIG_INPUT][S_TRIG_SOURCE], 2)); //-V525
    HAL_PIO::Write(PIN_A0S, _GET_BIT(datas[S_TRIG_INPUT][S_TRIG_SOURCE], 1));
    HAL_PIO::Write(PIN_LFS, _GET_BIT(datas[S_TRIG_INPUT][S_TRIG_SOURCE], 0));
}


void TrigInput::Set(E v)
{
    S_TRIG_INPUT = v;
    Load();
}


static bool NeedLoadRShift(Chan::E ch)
{
    bool result = true;

    static Range::E prevRanges[Chan::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Chan::Count] = { -1000, -1000 };

    if((prevShift[ch] == S_RSHIFT(ch)) && (prevRanges[ch] == S_RANGE(ch)))
    {
        result = false;
    }

    prevRanges[ch] = S_RANGE(ch);
    prevShift[ch] = S_RSHIFT(ch);

    return result;
}


void RShift::Load(Chan::E ch) //-V2506
{
    if(!NeedLoadRShift(ch))
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    int16 shift = S_RSHIFT(ch) + HARDWARE_ZERO;

    shift += NRST_EX_SHIFT(ch, S_RANGE(ch));

    if ((ch == ChanA) && Device::InModeTester())
    {
        shift -= Tester::DeltaRShiftA();
    }

    Osci::InputController::Write(PIN_SPI3_CS1, static_cast<uint16>(mask[ch] | (shift << 2)));

    Osci::Restart();

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        DataSettings *ds = RAM::Get();

        if(ds)
        {
            ds->Fill();
        }
    }
}


void FPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}


void TShift::LoadReal()
{
    FPGA::post = static_cast<uint16>(S_TIME_SHIFT - TShift().Min() + 10);
    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) - static_cast<int>(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = static_cast<uint16>(Pred);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred + 3));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::post);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::pred);
}


static int GetK()
{
    return (-TShift::Min()) % TBase::DeltaPoint();
}


void TShift::LoadRandomize()
{
    int k = TBase::DeltaPoint();

    FPGA::post = static_cast<uint16>((S_TIME_SHIFT - TShift().Min() - GetK()) / k);

    if((S_TIME_SHIFT - static_cast<int>(GetK())) < TShift().Min())
    {
        FPGA::post = 0;
    }

    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) / static_cast<int>(k) - static_cast<int>(FPGA::post);

    if (Pred < 5)
    {
        Pred = 5;
    }

    FPGA::pred = static_cast<uint16>(Pred);

    if(S_DBG_SHOW_RAND_PRED_POST)
    {
        LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post);
    }

    FPGA::post = static_cast<uint16>(~(FPGA::post + 10));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::pred);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::post);
}


void TShift::Load()
{
    if (OSCI_IN_MODE_RANDOMIZER)
    {
        LoadRandomize();
    }
    else
    {
        LoadReal();
    }
}


void TShift::Change(const int delta) //-V2506
{
    if (Device::InModeRecorder())
    {
        return;
    }

    TShift::Set(S_TIME_SHIFT + delta);

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TShift);
}


// ���������� range ��� ������ ������-����������
static void LimitForTester(Range::E *range)
{
    if(*range > Range::_5V)
    {
        *range = Range::_5V;
    }
    else if(*range < Range::_200mV) //-V2516
    {
        *range = Range::_200mV;
    }
}


void Range::Change(Chan::E ch, int16 delta) //-V2506
{
    if (Recorder::InRecordingMode())
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&S_RANGE(ch)), static_cast<uint8>(Range::Count - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&S_RANGE(ch)), 0);  // -V206
    }

    if(Device::InModeTester())
    {
        LimitForTester(&S_RANGE(ch));
    }

    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();

    DisplayOsci::DrawingValueParameter::Enable((ch == ChanA) ? DisplayOsci::DrawingValueParameter::RangeA : DisplayOsci::DrawingValueParameter::RangeB);
}


void RShift::Set(Chan::E ch, int16 rShift)
{
    ::Math::Limitation(&rShift, MIN, MAX);
    S_RSHIFT(ch) = rShift;
    Load(ch);
    DisplayOsci::DrawingValueParameter::Enable((ch == ChanA) ? DisplayOsci::DrawingValueParameter::RShiftA : DisplayOsci::DrawingValueParameter::RShiftB);
}


void RShift::Change(Chan::E ch, int16 delta)
{
    ::Math::AdditionThisLimitation<int16>(&S_RSHIFT(ch), STEP * delta, MIN, MAX);
    RShift::Load(ch);
    DisplayOsci::DrawingValueParameter::Enable((ch == ChanA) ? DisplayOsci::DrawingValueParameter::RShiftA : DisplayOsci::DrawingValueParameter::RShiftB);
}


void TrigPolarity::Load()
{
    FPGA::ForcedStart();
}


void TrigPolarity::Set(E v)
{
    S_TRIG_POLARITY = v;
    Load();
}


int TShift::Min()
{
#define k 0
#define mul 2

    static const struct StructENumPoints
    {
        int m[3];
        StructENumPoints(int m0, int m1, int m2) { m[0] = m0; m[1] = m1; m[2] = m2; }
    }
    m[ENumPointsFPGA::Count] =
    {
        StructENumPoints(-256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };

    return m[S_ENUMPOINTS_FPGA].m[S_TPOS];
}


int TShift::Max()
{
    return 60000;
}


String TShift::ToString(const TBase::E _base)
{
    TBase::E tBase = _base;

    if(tBase == TBase::Count)
    {
        tBase = S_TIME_BASE;
    }

    return Time(TShift::ToAbs(S_TIME_SHIFT, tBase)).ToString(true);
}


String RShift::ToString(int16 rShiftRel, Range::E range, Divider::E divider)
{
    float rShiftVal = ToAbs(rShiftRel, range) * Divider::ToAbs(divider); //-V2564
    return Voltage(rShiftVal).ToString(true);
}


pString TBase::Name(TBase::E tBase)
{
    static pString names[TBase::Count] =
    {
        "2��",
        "5��",
        "10��",
        "20��",
        "50��",
        "0.1���",
        "0.2���",
        "0.5���",
        "1���",
        "2���",
        "5���",
        "10���",
        "20���",
        "50���",
        "0.1��",
        "0.2��",
        "0.5��",
        "1��",
        "2��",
        "5��",
        "10��",
        "20��",
        "50��",
        "0.1�",
        "0.2�",
        "0.5�",
        "1�",
        "2�",
        "5�",
        "10�"
    };

    return names[tBase];
}


pString Range::Name(Chan::E ch)
{
    static const struct StructRange
    {
        const char * name;
        StructRange(pString nRU)
        {
            name = nRU;
        }
    }
    names[Range::Count] =
    {
        StructRange("2��"),
        StructRange("5��"),
        StructRange("10��"),
        StructRange("20��"),
        StructRange("50��"),
        StructRange("0.1�"),
        StructRange("0.2�"),
        StructRange("0.5�"),
        StructRange("1�"),
        StructRange("2�"),
        StructRange("5�"),
        StructRange("10�"),
        StructRange("20�")
    };

    return names[S_RANGE(ch)].name;
};


pString ModeCouple::UGO(ModeCouple::E v)
{
    static const pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[v];
}


void Bandwidth::Load(Chan::E ch)
{
    static const StructPIN pinsLF[2] = { {PIN_LF1}, {PIN_LF2} };

    HAL_PIO::Write(pinsLF[ch], S_BANDWIDTH_IS_20MHz(ch));
}


void ModeCouple::Set(Chan::E ch, ModeCouple::E couple)
{
    S_MODE_COUPLE(ch) = couple;
    Range::LoadBoth();
}
