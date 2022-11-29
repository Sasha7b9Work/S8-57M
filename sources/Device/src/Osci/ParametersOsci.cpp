#include "defines.h"
#include "globals.h"
#include "device.h"
#include "MessageMgr.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


#ifdef WIN32
#pragma warning(disable:4310)
#endif


//                                            2�� 5�� 10�� 20�� 50��
static const int deltaPoint[TBase::Count] = { 50, 20, 10,  5,   2 };


static const int voltsInPixelInt[] =   // ����������� 20000
{
    2,      // 2
    5,      // 5
    10,     // 10
    20,     // 20
    50,     // 50
    100,    // 100
    200,    // 200
    500,    // 500
    1000,   // 1
    2000,   // 2
    5000,   // 5
    1000,   // 10
    20000   // 20
};


const float VALUE::voltsInPoint[Range::Count] =
{
    2e-3F   / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 2mV //-V2564
    5e-3F   / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 5mV //-V2564
    10e-3F  / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 10mV //-V2564
    20e-3F  / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 20mV //-V2564
    50e-3F  / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 50mV //-V2564
    100e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 100mV //-V2564
    200e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 200mV //-V2564
    500e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 500mV //-V2564
    1.0F    / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 1V //-V2564
    2.0F    / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 2V //-V2564
    5.0F    / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 5V //-V2564
    10.0F   / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 10V //-V2564
    20.0F   / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN)     // 20V //-V2564
};


// ������ �������� �������� ��������� �� ����������.
static const struct RangeStruct
{
    pString name;     // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
    RangeStruct(pString nRU) : name(nRU) {};
}
ranges[Range::Count][2] =
{
    {RangeStruct("2\x10��"),  RangeStruct("20\x10��")},
    {RangeStruct("5\x10��"),  RangeStruct("50\x10��")},
    {RangeStruct("10\x10��"), RangeStruct("0.1\x10�")},
    {RangeStruct("20\x10��"), RangeStruct("0.2\x10�")},
    {RangeStruct("50\x10��"), RangeStruct("0.5\x10�")},
    {RangeStruct("0.1\x10�"), RangeStruct("1\x10�")},
    {RangeStruct("0.2\x10�"), RangeStruct("2\x10�")},
    {RangeStruct("0.5\x10�"), RangeStruct("5\x10�")},
    {RangeStruct("1\x10�"),   RangeStruct("10\x10�")},
    {RangeStruct("2\x10�"),   RangeStruct("20\x10�")},
    {RangeStruct("5\x10�"),   RangeStruct("50\x10�")},
    {RangeStruct("10\x10�"),  RangeStruct("100\x10�")},
    {RangeStruct("20\x10�"),  RangeStruct("200\x10�")}
};


const float RShift::absStep[Range::Count] =
{
    2e-3F   / 20 / RShift::STEP, //-V2564
    5e-3F   / 20 / RShift::STEP, //-V2564
    10e-3F  / 20 / RShift::STEP, //-V2564
    20e-3F  / 20 / RShift::STEP, //-V2564
    50e-3F  / 20 / RShift::STEP, //-V2564
    100e-3F / 20 / RShift::STEP, //-V2564
    200e-3F / 20 / RShift::STEP, //-V2564
    500e-3F / 20 / RShift::STEP, //-V2564
    1.0F    / 20 / RShift::STEP, //-V2564
    2.0F    / 20 / RShift::STEP, //-V2564
    5.0F    / 20 / RShift::STEP, //-V2564
    10.0F   / 20 / RShift::STEP, //-V2564
    20.0F   / 20 / RShift::STEP //-V2564
};


const float TShift::absStep[TBase::Count] =
{
    /* 2 ��    */ 2e-9F   / 20, //-V2564
    /* 5 ��    */ 5e-9F   / 20, //-V2564
    /* 10 ��   */ 10e-9F  / 20, //-V2564
    /* 20 ��   */ 20e-9F  / 20, //-V2564
    /* 50 ��   */ 50e-9F  / 20,   // 1.0  ��� ������������ ��� ���������� ��������� ������������ �������� //-V2564
    /* 100 ��  */ 100e-9F / 20,   // 2.0 //-V2564
    /* 200 ��  */ 200e-9F / 20,   // 4.0 //-V2564
    /* 500 ��  */ 500e-9F / 20,   // 10.0 //-V2564
    /* 1 ���   */ 1e-6F   / 20,   // 20.0 //-V2564
    /* 2 ���   */ 2e-6F   / 20,   // 40.0 //-V2564
    /* 5 ���   */ 5e-6F   / 20,   // 100.0 //-V2564
    /* 10 ���  */ 10e-6F  / 20,   // 200.0 //-V2564
    /* 20 ���  */ 20e-6F  / 20,   // 400.0 //-V2564
    /* 50 ���  */ 50e-6F  / 20,   // 1e3 //-V2564
    /* 100 ��� */ 100e-6F / 20,   // 2e3 //-V2564
    /* 200 ��� */ 200e-6F / 20,   // 4e3 //-V2564
    /* 500 ��� */ 500e-6F / 20,   // 10e3 //-V2564
    /* 1 ��    */ 1e-3F   / 20,   // 20e3 //-V2564
    /* 2 ��    */ 2e-3F   / 20,   // 40e3 //-V2564
    /* 5 ��    */ 5e-3F   / 20,   // 100e3 //-V2564
    /* 10 ��   */ 10e-3F  / 20,   // 200e3 //-V2564
    /* 20 ��   */ 20e-3F  / 20,   // 400e3 //-V2564
    /* 50 ��   */ 50e-3F  / 20,   // 1e4 //-V2564
    /* 100 ��  */ 100e-3F / 20,   // 2e4 //-V2564
    /* 200 ��  */ 200e-3F / 20,   // 4e4 //-V2564
    /* 500 ��  */ 500e-3F / 20,   // 10e4 //-V2564
    /* 1 �     */ 1.0F    / 20,   // 20e4 //-V2564
    /* 2 �     */ 2.0F    / 20,   // 40e4 //-V2564
    /* 5 �     */ 5.0F    / 20,   // 100e4 //-V2564
    /* 10 �    */ 10.0F   / 20    // 200e4 //-V2564
};


static uint8 ValueForRange(Chan::E ch);


//void Osci::LoadHoldfOff()
//{
//    HAL_BUS::FPGA::Write8(WR::TRIG_HOLD_ENABLE, S_TRIG_HOLDOFF_ENABLED ? 1U : 0U);
//
//    uint value = static_cast<uint>(0 - S_TRIG_HOLDOFF_VALUE + 1);
//
//    BitSet32 bs(value);
//
//    HAL_BUS::FPGA::Write8(WR::TRIG_HOLD_VALUE_LOW, bs.byte0);
//    HAL_BUS::FPGA::Write8(WR::TRIG_HOLD_VALUE_MID, bs.byte1);
//    HAL_BUS::FPGA::Write8(WR::TRIG_HOLD_VALUE_HI, bs.byte2);
//}


void TBase::Change(int delta) //-V2506
{
    TBase::E old = S_TIME_BASE;

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&S_TIME_BASE), static_cast<uint8>(TBase::Count - 1));
    }
    else
    {
        if (PeakDetMode().IsEnabled() &&                            // ���� ������ ����� �������� ���������
            S_TIME_BASE == TBase::MIN_PEAK_DET)                     // � ���������� ������� �� �������, ��������������� ����������� � ������ �������� ��������� :
        {
            ::Display::ShowWarning("������� ������� ��������");		// ������� ��������� �� ����
            return;													// � �������
        }

        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&S_TIME_BASE), 0);
    }

    if (old == S_TIME_BASE)
    {
        return;
    }

    Set();

    Osci::Restart();

    DisplayOsci::SetFlagRedraw();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TBase);
}


pString TBase::ToString(TBase::E tBase)
{
    // ��������� ��� �������� ��������� �������� �� �������.
    static const struct StructTBase
    {
        const char *name;    // �������� ��������� � ��������� ����, ��������� ��� ������ �� �����.
        StructTBase(pString nRU)
        {
            name = nRU;
        };
    }
    tBases[Count] =
    {
        StructTBase("2\x10��"),
        StructTBase("5\x10��"),
        StructTBase("10\x10��"),
        StructTBase("20\x10��"),
        StructTBase("50\x10��"),
        StructTBase("0.1\x10���"),
        StructTBase("0.2\x10���"),
        StructTBase("0.5\x10���"),
        StructTBase("1\x10���"),
        StructTBase("2\x10���"),
        StructTBase("5\x10���"),
        StructTBase("10\x10���"),
        StructTBase("20\x10���"),
        StructTBase("50\x10���"),
        StructTBase("0.1\x10��"),
        StructTBase("0.2\x10��"),
        StructTBase("0.5\x10��"),
        StructTBase("1\x10��"),
        StructTBase("2\x10��"),
        StructTBase("5\x10��"),
        StructTBase("10\x10��"),
        StructTBase("20\x10��"),
        StructTBase("50\x10��"),
        StructTBase("0.1\x10�"),
        StructTBase("0.2\x10�"),
        StructTBase("0.5\x10�"),
        StructTBase("1\x10�"),
        StructTBase("2\x10�"),
        StructTBase("5\x10�"),
        StructTBase("10\x10�")
    };

    return tBases[tBase].name;
}


void TBase::Set(TBase::E base)
{
    if (base != Count)
    {
        S_TIME_BASE = base;
    }


    static const uint8 values[TBase::Count] =
    {
        BIN_U8(00000000),  // -V2501  // 2ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 5ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 10ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 20ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 50ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 100ns   1       200MHz
        BIN_U8(00100000),  // -V2501  // 200ns   2       100MHz
        BIN_U8(00100001),  // -V2501  // 500ns   5       40MHz
        BIN_U8(00100010),  // -V2501  // 1us     10      20MHz
        BIN_U8(00100011),  // -V2501  // 2us     20      10MHz
        BIN_U8(01000101),  // -V2501  // 5us     50      4MHz
        BIN_U8(01000110),  // -V2501  // 10us    100     2MHz
        BIN_U8(01000111),  // -V2501  // 20us    200     1MHz
        BIN_U8(01001001),  // -V2501  // 50us    500     400kHz
        BIN_U8(01001010),  // -V2501  // 100us   1k      200kHz
        BIN_U8(01001011),  // -V2501  // 200us   2k      100kHz
        BIN_U8(01001101),  // -V2501  // 500us   5k      40kHz
        BIN_U8(01001110),  // -V2501  // 1ms     10k     20kHz
        BIN_U8(01001111),  // -V2501  // 2ms     20k     10kHz
        BIN_U8(01010001),  // -V2501  // 5ms     50k     4kHz
        BIN_U8(01010010),  // -V2501  // 10ms    100k    2kHz
        BIN_U8(01010011),  // -V2501  // 20ms    200k    1kHz
        BIN_U8(01010101),  // -V2501  // 50ms    500k    400Hz
        BIN_U8(01010110),  // -V2501  // 100ms   1M      200Hz
        BIN_U8(01010111),  // -V2501  // 200ms   2M      100Hz
        BIN_U8(01011001),  // -V2501  // 500ms   5M      40Hz
        BIN_U8(01011010),  // -V2501  // 1s      10M     20Hz
        BIN_U8(01011011),  // -V2501  // 2s      20M     10Hz
        BIN_U8(01011101),  // -V2501  // 5s      50M     4Hz
        BIN_U8(01011110)   // -V2501  // 10s     100M    2Hz
    };

    Osci::ClearDataRand();

    HAL_BUS::FPGA::Write8(WR::TBASE, values[S_TIME_BASE]);

    FPGA::LoadRegUPR();

    TShift::Load();

    Bandwidth::Load(ChanA);
    Bandwidth::Load(ChanB);

    MessageMgr::OsciSettingsEffectOnAverageChanged();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TBase);
}


void Range::LoadBoth()
{
    uint16 val = static_cast<uint16>(ValueForRange(ChanB) + (ValueForRange(ChanA) << 8));

    Osci::InputController::Write(PIN_SPI3_CS2, val);

    PAUSE_ON_MS(10);                // �������� �����, ����� ���������� ���� ������ ����������

    Osci::InputController::Write(PIN_SPI3_CS2, 0);    // ���������� ����, ����� ���� �� ���������� �������

    //DEF__STRUCT(StructRange, uint8) vals[Range::Count] =
    static const uint8 vals[Range::Count] =
    {
        BIN_U8(00000000),  // 2mV      // -V2501
        BIN_U8(00000001),  // 5mV      // -V2501
        BIN_U8(00000010),  // 10mV     // -V2501
        BIN_U8(00000011),  // 20mV     // -V2501
        BIN_U8(00000001),  // 50mV     // -V2501
        BIN_U8(00000010),  // 100mV    // -V2501
        BIN_U8(00000011),  // 200mV    // -V2501
        BIN_U8(00000001),  // 500mV    // -V2501
        BIN_U8(00000010),  // 1V       // -V2501
        BIN_U8(00000011),  // 2V       // -V2501
        BIN_U8(00000001),  // 5V       // -V2501
        BIN_U8(00000010),  // 10V      // -V2501
        BIN_U8(00000011)   // 20V      // -V2501
    };

    uint8 valueA = vals[S_RANGE_A];

    HAL_PIO::Write(PIN_A1, _GET_BIT(valueA, 1));
    HAL_PIO::Write(PIN_A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[S_RANGE_B];

    HAL_PIO::Write(PIN_A3, _GET_BIT(valueB, 1));
    HAL_PIO::Write(PIN_A4, _GET_BIT(valueB, 0));

    Bandwidth::Load(ChanA);
    Bandwidth::Load(ChanB);

    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        RShift::Load(ChanB);
        RShift::Load(ChanA);
    }
    else
    {
        RShift::Load(ChanA);
        RShift::Load(ChanB);
    }

    Osci::Restart();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


pString Range::ToString(Chan::E ch, Divider::E divider)
{
    return ranges[S_RANGE(ch)][divider].name;
}


pString Range::ToString(Range::E range, Divider::E _divider)
{
    return ranges[range][_divider].name;
}


static uint8 ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Count] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    //DEF__STRUCT(StructRange, uint16) values[Range::Count][2] =
    static const uint16 values[Range::Count][2] =
    {   //             A                    B
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 2mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 5mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 10mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 20mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 50mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 100mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 200mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 500mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 1V
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 2V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 5V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 10V
        { BIN_U8(00011000), BIN_U8(00011000) }  // -V2501  // 20V
    };

    ModeCouple::E couple = (Device::InModeRecorder()) ? ModeCouple::DC : S_MODE_COUPLE(ch);

    if (Device::InModeOsci() && couple == ModeCouple::GND)
    {
        return datas[ModeCouple::GND];
    }

    return static_cast<uint8>(values[S_RANGE(ch)][ch] | datas[couple]);
}


void TrigLevel::Find()
{
    Chan::E ch = S_TRIG_SOURCE;

    if (DS && ENABLED_DS(ch))
    {
        const uint8 *data = IN(ch);

        int numBytes = DS->BytesInChannel();

        uint8 max = Math::MaxFromArray(data, 0, static_cast<int>(numBytes) - 1);
        uint8 min = Math::MinFromArray(data, 0, static_cast<int>(numBytes) - 1);

        int deltaValue = static_cast<int>(VALUE::AVE) - (max + min) / 2;

        int deltaRShift = S_RSHIFT(ch);

        float k = 200 / 125.0F;     // ���� ����������� ���������� ���, ��� �� ������ ������� ������ ����� 125-� ����� ������� �� ������ ������ (�������� ��������), //-V2564
                                    // � ������ � ���� ����� ������ �� 200 ������ ������������ ����� ������

        float additionShift = deltaValue + deltaRShift / k;     // �������� �������, ������� ����� �������� � TrigLev::Zero //-V2564

        TrigLevel::Set(ch, static_cast<int16>(-additionShift * k + 0.5F));
    }
}


void RShift::DrawBoth()
{
    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        RShift::Draw(ChanB);
        RShift::Draw(ChanA);
    }
    else
    {
        RShift::Draw(ChanA);
        RShift::Draw(ChanB);
    }
}


pString Chan::Name(Chan::E ch)
{
    static pString names[Chan::Count] =
    {
        "����� 1",
        "����� 2"
    };

    return names[ch];
}


void TrigLevel::Load()
{
    // \todo ����� ����� ������ ��������. ����� ���-�� ������� � ������ SET_TRIGLEV_SOURCE
    uint16 value = static_cast<uint16>(HARDWARE_ZERO - S_TRIG_LEVEL_SOURCE);

    Osci::InputController::Write(PIN_SPI3_CS1, static_cast<uint16>(0xa000 | (value << 2)));

    Osci::Restart();
}


void TrigLevel::Change(int16 delta)
{
    Math::AdditionThisLimitation(&S_TRIG_LEVEL_SOURCE, TrigLevel::STEP * delta, TrigLevel::MIN, TrigLevel::MAX);

    Load();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TrigLevel);
}


void TrigLevel::Set(Chan::E ch, int16 newLevel)
{
    S_TRIG_LEVEL(ch) = newLevel;

    Math::Limitation(&S_TRIG_LEVEL(ch), TrigLevel::MIN, TrigLevel::MAX);

    Load();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TrigLevel);
}


bool Trig::SyncPulse()
{
    return Trig::pulse;
}


void RShift::Draw(Chan::E ch)
{
    Color::CHAN[ch].SetAsCurrent();

    int delta = S_RSHIFT(ch) / STEP;

    if(S_FFT_ENABLED)
    {
        delta /= 2;
    }

    int y = (Grid::ChannelBottom() - Grid::Top()) / 2 + Grid::Top() - delta;
    int x = Grid::Left() - 8;

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol8::TRIGLEV_LOWER).Draw(x + 1, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 3, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x += 1;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol8::TRIGLEV_ABOVE).Draw(x + 1, Grid::Top() + 2);
        Pixel().Draw(x + 3, Grid::Top() + 2);
        y = Grid::Top() + 7;
        x += 1;
    }
    else
    {
        Char(Symbol8::RSHIFT_NORMAL).Draw(x, y - 4);
    }

    Char((ch == ChanA) ? '1' : '2', DTypeFont::_5).Draw(x + 1, y - 6, Color::BACK);
}


void TrigLevel::Draw()
{
    Chan::E ch = S_TRIG_SOURCE;

    float scale = 1.0F / ((MAX - MIN) / 2.4F / Grid::Height()); //-V2564

    int y = Grid::ChannelCenterHeight() - static_cast<int>((S_TRIG_LEVEL(ch) + S_RSHIFT(ch)) * scale); //-V2564

    int x = Grid::Right();
    int xSymbol = Grid::Right() + 5;

    Color::Trig().SetAsCurrent();

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol8::TRIGLEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        xSymbol--;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol8::TRIGLEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Pixel().Draw(x + 5, Grid::Top() + 2);
        y = Grid::Top() + 7;
        xSymbol--;
    }
    else
    {
        Char(Symbol8::TRIGLEV_NORMAL).Draw(x + 1, y - 4);
    }

    static const char symbols[2] = { '1', '2' };

    Char(symbols[ch], DTypeFont::_5).Draw(xSymbol, y - 6, Color::BACK);
}


float RShift::ToAbs(int16 rShift, Range::E range)
{
    return (rShift * absStep[range]); //-V2564
}


int16 RShift::ToRel(float rShiftAbs, Range::E range)
{
    int16 retValue = static_cast<int16>(rShiftAbs / absStep[range]);

    if (retValue < MIN)
    {
        retValue = MIN;
    }
    else if (retValue > MAX) //-V2516
    {
        retValue = MAX;
    }

    return retValue;
}


bool RShift::ChangeMath(int delta) //-V2506
{
    int16 prevRShift = S_RSHIFT_MATH;
    int16 rShift = prevRShift;

    if (delta > 0)
    {

        if (rShift < MAX)
        {
            rShift += 4 * STEP;  // -V112
            LIMIT_ABOVE(rShift, MAX);
            if (prevRShift < 0 && rShift > 0)
            {
                rShift = 0;
            }
            S_RSHIFT_MATH = rShift;
            return true;
        }
    }
    else
    {
        if (rShift > MIN)
        {
            rShift -= 4 * STEP; // -V112
            LIMIT_BELOW(rShift, MIN);
            if (prevRShift > 0 && rShift < 0)
            {
                rShift = 0;
            }
            S_RSHIFT_MATH = rShift;
            return true;
        }
    }

    return false;
}


float TShift::ToAbs(const int tShift, const TBase::E tBase)
{
    return absStep[tBase] * tShift; //-V2564
}

void VALUE::PointsToVoltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = Range::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    int diff = static_cast<int>((MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F); //-V2564
    float koeff = 1.0F / 20e3F;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff; //-V636 //-V2563 //-V2564
    }
}


float Range::MaxVoltageOnScreen(Range::E range)
{
    //DEF__STRUCT(StructRange, float) table[Range::Count] =
    static const float table[Range::Count] =
    {
        2e-3F, 5e-3F, 10e-3F, 20e-3F, 50e-3F, 100e-3F, 200e-3F, 500e-3F, 1.0F, 2.0F, 5.0F, 10.0F, 20.0F
    };

    return table[range] * 5.0F;
}


uint8 VALUE::FromVoltage(float voltage, Range::E range, int16 rShift)
{
    int relValue = static_cast<int>((voltage + Range::MaxVoltageOnScreen(range) + RShift::ToAbs(rShift, range)) / voltsInPoint[range] + MIN); //-V2564
    ::Math::Limitation<int>(&relValue, 0, 255);
    return static_cast<uint8>(relValue);
}


float VALUE::ToVoltage(uint8 value, Range::E range, int16 rShift)
{
    uint8 delta = static_cast<uint8>(value - MIN);

    float rShiftAbs = RShift::ToAbs(rShift, range);

    float maxVoltage = Range::MaxVoltageOnScreen(range);

    return delta * voltsInPoint[range] - maxVoltage - rShiftAbs; //-V2564
}


void VALUE::PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points)
{
    float maxVoltOnScreen = Range::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    float voltInPixel = 1.0F / (voltsInPoint[range] / ((MAX - MIN) / 200.0F)); //-V2564

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + MIN; //-V2564

    for (int i = 0; i < numPoints; i++)
    {
        int value = static_cast<int>(voltage[i] * voltInPixel + delta); //-V2563

        if (value < 0)
        {
            points[i] = 0; //-V2563
            continue;
        }
        else if (value > 255) //-V2516
        {
            points[i] = 255; //-V2563
            continue;
        }

        points[i] = static_cast<uint8>(value); //-V2563
    }
}


void Range::Set(Chan::E ch, E range)
{
    set.disp.SetLastAffectedChannel(ch);

    S_RANGE(ch) = range;

    LoadBoth();

    DisplayOsci::DrawingValueParameter::Enable((ch == ChanA) ? DisplayOsci::DrawingValueParameter::RangeA : DisplayOsci::DrawingValueParameter::RangeB);
}


int TBase::DeltaPoint()
{
    return OSCI_IN_MODE_RANDOMIZER ? deltaPoint[S_TIME_BASE] : 1;
}


float TBase::TimePoint(TBase::E base)
{
    return TShift::ToAbs(1, base);
}


void TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max());

    S_TIME_SHIFT = tShift;

    Load();

    Osci::Restart();

    DisplayOsci::SetFlagRedraw();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TShift);
}


void TShift::Reset()
{
    TShift::Set(0);
}


void TShift::Draw()
{
    int x = TPos().PosX() - S_TIME_SHIFT;

    if (x < Grid::Left())
    {
        DrawLeft();
    }
    else if (x > Grid::Right())
    {
        DrawRight();
    }
    else
    {
        DrawNormal(x, Grid::Top() - 1);
    }
}


void TShift::DrawNormal(const int x, const int y)
{
    Char(Symbol8::TSHIFT_NORMAL_BACK).Draw(x - 3, y, Color::BACK);
    Char(Symbol8::TSHIFT_NORMAL).Draw(x - 3, y, Color::FILL);
}


void TShift::DrawLeft()
{
    Char(Symbol8::TRIGLEV_NORMAL).Draw(Grid::Left(), Grid::Top(), Color::BACK);
    Char(Symbol8::TSHIFT_LEFT).Draw(Grid::Left(), Grid::Top(), Color::FILL);
}

void TShift::DrawRight()
{
    Char(Symbol8::RSHIFT_NORMAL).Draw(Grid::Right() - 7, Grid::Top(), Color::BACK);
    Char(Symbol8::TSHIFT_RIGHT).Draw(Grid::Right() - 7, Grid::Top(), Color::FILL);
}
