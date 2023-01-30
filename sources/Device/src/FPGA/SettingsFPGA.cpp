// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
#include "Hardware/HAL/HAL_PIO.h"


namespace RShift
{
    // Это знаение, которое нужно засылать в аппаратную часть, чтобы получить смещение "0"
    static const int16 HARDWARE_ZERO = 500;

    extern const int16 MIN = -480;
    extern const int16 MAX = 480;
    extern const int16 STEP = (((MAX - MIN) / 24) / 20);
}


namespace TShift
{
    static void LoadReal();

    static void LoadRandomize();
}


void TrigStartMode::Set(TrigStartMode::E v)
{
    S_TRIG_START_MODE = v;
    Osci::ChangedTrigStartMode();
}


void TrigSource::Set(Ch::E ch)
{
    if (ch != Ch::Count)
    {
        S_TRIG_SOURCE = ch;
    }

    TrigInput::Load();
}


void TrigInput::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ПС 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // НЧ
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


static bool NeedLoadRShift(Ch::E ch)
{
    bool result = true;

    static Range::E prevRanges[Ch::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Ch::Count] = { -1000, -1000 };

    if((prevShift[ch] == S_RSHIFT(ch)) && (prevRanges[ch] == S_RANGE(ch)))
    {
        result = false;
    }

    prevRanges[ch] = S_RANGE(ch);
    prevShift[ch] = S_RSHIFT(ch);

    return result;
}


void RShift::Load(Ch::E ch)
{
    if(!NeedLoadRShift(ch))
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    int16 shift = S_RSHIFT(ch) + HARDWARE_ZERO;

    shift += NRST_EX_SHIFT(ch, S_RANGE(ch));

    Osci::InputController::Write(PIN_SPI3_CS1, (uint16)(mask[ch] | (shift << 2)));

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
    FPGA::post = (uint16)(S_TIME_SHIFT - TShift::Min() + 10);
    int Pred = (int)(ENumPointsFPGA::PointsInChannel()) - (int)(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)(Pred);

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

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

    FPGA::post = (uint16)((S_TIME_SHIFT - TShift::Min() - GetK()) / k);

    if((S_TIME_SHIFT - (int)(GetK())) < TShift::Min())
    {
        FPGA::post = 0;
    }

    int Pred = (int)(ENumPointsFPGA::PointsInChannel()) / (int)(k) - (int)(FPGA::post);

    if (Pred < 5)
    {
        Pred = 5;
    }

    FPGA::pred = (uint16)(Pred);

    if(S_DBG_SHOW_RAND_PRED_POST)
    {
        LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post);
    }

    FPGA::post = (uint16)(~(FPGA::post + 10));
    FPGA::pred = (uint16)(~(FPGA::pred));

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


void TShift::Change(const int delta)
{
    if (Device::InModeRecorder())
    {
        return;
    }

    TShift::Set(S_TIME_SHIFT + delta);

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TShift);
}


void Range::Change(Ch::E ch, int16 delta)
{
    if (Recorder::InRecordingMode())
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)&S_RANGE(ch), (uint8)(Range::Count - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)&S_RANGE(ch), 0);  // -V206
    }

    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();

    DisplayOsci::DrawingValueParameter::Enable((ch == ChA) ? DisplayOsci::DrawingValueParameter::RangeA : DisplayOsci::DrawingValueParameter::RangeB);
}


void RShift::Set(Ch::E ch, int16 rShift)
{
    ::Math::Limitation(&rShift, RShift::MIN, MAX);
    S_RSHIFT(ch) = rShift;
    Load(ch);
    DisplayOsci::DrawingValueParameter::Enable((ch == ChA) ? DisplayOsci::DrawingValueParameter::RShiftA : DisplayOsci::DrawingValueParameter::RShiftB);
}


void RShift::Change(Ch::E ch, int16 delta)
{
    ::Math::AdditionThisLimitation<int16>(&S_RSHIFT(ch), STEP * delta, MIN, MAX);
    RShift::Load(ch);
    DisplayOsci::DrawingValueParameter::Enable((ch == ChA) ? DisplayOsci::DrawingValueParameter::RShiftA : DisplayOsci::DrawingValueParameter::RShiftB);
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
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),     // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),    // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),    // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)     // 8192
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
    float rShiftVal = ToAbs(rShiftRel, range) * Divider::ToAbs(divider);
    return Voltage(rShiftVal).ToString(true);
}


pcharc TBase::Name(TBase::E tBase)
{
    static pcharc names[TBase::Count] =
    {
        "2нс",
        "5нс",
        "10нс",
        "20нс",
        "50нс",
        "0.1мкс",
        "0.2мкс",
        "0.5мкс",
        "1мкс",
        "2мкс",
        "5мкс",
        "10мкс",
        "20мкс",
        "50мкс",
        "0.1мс",
        "0.2мс",
        "0.5мс",
        "1мс",
        "2мс",
        "5мс",
        "10мс",
        "20мс",
        "50мс",
        "0.1с",
        "0.2с",
        "0.5с",
        "1с",
        "2с",
        "5с",
        "10с"
    };

    return names[tBase];
}


pcharc Range::Name(Ch::E ch)
{
    static const struct StructRange
    {
        pchar  name;
        StructRange(pcharc nRU)
        {
            name = nRU;
        }
    }
    names[Range::Count] =
    {
        StructRange("2мВ"),
        StructRange("5мВ"),
        StructRange("10мВ"),
        StructRange("20мВ"),
        StructRange("50мВ"),
        StructRange("0.1В"),
        StructRange("0.2В"),
        StructRange("0.5В"),
        StructRange("1В"),
        StructRange("2В"),
        StructRange("5В"),
        StructRange("10В"),
        StructRange("20В")
    };

    return names[S_RANGE(ch)].name;
};


SymbolUGO ModeCouple::UGO(ModeCouple::E v)
{
    static const SymbolUGO couple[] =
    {
        SymbolUGO(SymbolUGO::COUPLE_DC),
        SymbolUGO(SymbolUGO::COUPLE_AC),
        SymbolUGO(SymbolUGO::COUPLE_GND)
    };

    return couple[v];
}


void Bandwidth::Load(Ch::E ch)
{
    static const StructPIN pinsLF[2] = { {PIN_LF1}, {PIN_LF2} };

    HAL_PIO::Write(pinsLF[ch], S_BANDWIDTH_IS_20MHz(ch));
}


void ModeCouple::Set(Ch::E ch, ModeCouple::E couple)
{
    S_MODE_COUPLE(ch) = couple;
    Range::LoadBoth();
}
