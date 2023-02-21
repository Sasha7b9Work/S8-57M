// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "globals.h"
#include "device.h"
#include "MessageMgr.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "common/Display/Font/Symbols.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Hardware/HAL/HAL_PIO.h"


#ifdef WIN32
#pragma warning(disable:4310)
#endif


//                                            2нс 5нс 10нс 20нс 50нс
static const int deltaPoint[TBase::Count] = { 50, 20, 10,  5,   2 };


static const int voltsInPixelInt[] =   // Коэффициент 20000
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


namespace VALUE
{
    const float voltsInPoint[Range::Count] =
    {
        2e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 2mV
        5e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 5mV
        10e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 10mV
        20e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 20mV
        50e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 50mV
        100e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 100mV
        200e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 200mV
        500e-3F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 500mV
        1.0F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 1V
        2.0F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 2V
        5.0F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 5V
        10.0F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN),    // 10V
        20.0F / 20 * Grid::Height() / (VALUE::MAX - VALUE::MIN)     // 20V
    };
}


// Массив структур описаний масштабов по напряжению.
static const struct RangeStruct
{
    pcharc name;     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    RangeStruct(pcharc nRU) : name(nRU) {};
}
ranges[Range::Count][2] =
{
    {RangeStruct("2мВ"),  RangeStruct("20мВ")},
    {RangeStruct("5мВ"),  RangeStruct("50мВ")},
    {RangeStruct("10мВ"), RangeStruct("0.1В")},
    {RangeStruct("20мВ"), RangeStruct("0.2В")},
    {RangeStruct("50мВ"), RangeStruct("0.5В")},
    {RangeStruct("0.1В"), RangeStruct("1В")},
    {RangeStruct("0.2В"), RangeStruct("2В")},
    {RangeStruct("0.5В"), RangeStruct("5В")},
    {RangeStruct("1В"),   RangeStruct("10В")},
    {RangeStruct("2В"),   RangeStruct("20В")},
    {RangeStruct("5В"),   RangeStruct("50В")},
    {RangeStruct("10В"),  RangeStruct("100В")},
    {RangeStruct("20В"),  RangeStruct("200В")}
};


namespace RShift
{
    // Это значение соответствует минимуму смещения
    extern const int16 MIN;

    // Это значение соотвествует максимуму смещения
    extern const int16 MAX;

    // На столько единиц нужно изменить значение смещения, чтобы маркер смещения по напряжению передвинулся на одну точку.
    extern const int16 STEP;

    const float absStep[Range::Count] =
    {
        2e-3F / 20 / RShift::STEP,
        5e-3F / 20 / RShift::STEP,
        10e-3F / 20 / RShift::STEP,
        20e-3F / 20 / RShift::STEP,
        50e-3F / 20 / RShift::STEP,
        100e-3F / 20 / RShift::STEP,
        200e-3F / 20 / RShift::STEP,
        500e-3F / 20 / RShift::STEP,
        1.0F / 20 / RShift::STEP,
        2.0F / 20 / RShift::STEP,
        5.0F / 20 / RShift::STEP,
        10.0F / 20 / RShift::STEP,
        20.0F / 20 / RShift::STEP
    };

    // Отрисовать маркер вертикального смещения на сетке
    static void Draw(Ch::E ch);
}


namespace TShift
{
    // Нарисовать "нормальное" изображение маркера смещения, когда позиция смещения находится на экране
    static void DrawNormal(const int x, const int y);

    // Нарисовать маркер смещения, когда позиция смещения находится за левой границей экрана
    static void DrawLeft();

    // Нарисовать маркер смещения, когда позиция смещения находится за правой границей экрана
    static void DrawRight();

    const float absStep[TBase::Count] =
    {
        /* 2 нс    */ 2e-9F / 20,
        /* 5 нс    */ 5e-9F / 20,
        /* 10 нс   */ 10e-9F / 20,
        /* 20 нс   */ 20e-9F / 20,
        /* 50 нс   */ 50e-9F / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
        /* 100 нс  */ 100e-9F / 20,   // 2.0
        /* 200 нс  */ 200e-9F / 20,   // 4.0
        /* 500 нс  */ 500e-9F / 20,   // 10.0
        /* 1 мкс   */ 1e-6F / 20,   // 20.0
        /* 2 мкс   */ 2e-6F / 20,   // 40.0
        /* 5 мкс   */ 5e-6F / 20,   // 100.0
        /* 10 мкс  */ 10e-6F / 20,   // 200.0
        /* 20 мкс  */ 20e-6F / 20,   // 400.0
        /* 50 мкс  */ 50e-6F / 20,   // 1e3
        /* 100 мкс */ 100e-6F / 20,   // 2e3
        /* 200 мкс */ 200e-6F / 20,   // 4e3
        /* 500 мкс */ 500e-6F / 20,   // 10e3
        /* 1 мс    */ 1e-3F / 20,   // 20e3
        /* 2 мс    */ 2e-3F / 20,   // 40e3
        /* 5 мс    */ 5e-3F / 20,   // 100e3
        /* 10 мс   */ 10e-3F / 20,   // 200e3
        /* 20 мс   */ 20e-3F / 20,   // 400e3
        /* 50 мс   */ 50e-3F / 20,   // 1e4
        /* 100 мс  */ 100e-3F / 20,   // 2e4
        /* 200 мс  */ 200e-3F / 20,   // 4e4
        /* 500 мс  */ 500e-3F / 20,   // 10e4
        /* 1 с     */ 1.0F / 20,   // 20e4
        /* 2 с     */ 2.0F / 20,   // 40e4
        /* 5 с     */ 5.0F / 20,   // 100e4
        /* 10 с    */ 10.0F / 20    // 200e4
    };
}


static uint8 ValueForRange(Ch::E ch);


namespace TrigLevel
{
    static const int16 MIN = -480;
    static const int16 MAX = 480;
    static const int16 HARDWARE_ZERO = 500;
    static const int16 STEP = (((MAX - MIN) / 24) / 20);
}


void TBase::Change(int delta)
{
    TBase::E old = S_TIME_BASE;

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)&S_TIME_BASE, (uint8)(TBase::Count - 1));
    }
    else
    {
        if (PeakDetMode().IsEnabled() &&                            // Если вклюён режим пикового детектора
            S_TIME_BASE == TBase::MIN_PEAK_DET)                     // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            ::Display::ShowWarning("ВКЛЮЧЕН ПИКОВЫЙ ДЕТЕКТОР");     // выводим сообщение об этом
            return;                                                 // и выходим
        }

        ::Math::LimitationDecrease<uint8>((uint8 *)&S_TIME_BASE, 0);
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


pcharc TBase::ToString(TBase::E tBase)
{
    // Структура для описания диапазона масштаба по времени.
    static const struct StructTBase
    {
        pchar name;    // Название диапазона в текстовом виде, пригодном для вывода на экран.
        StructTBase(pcharc nRU)
        {
            name = nRU;
        };
    }
    tBases[Count] =
    {
        StructTBase("2 нс"),
        StructTBase("5 нс"),
        StructTBase("10 нс"),
        StructTBase("20 нс"),
        StructTBase("50 нс"),
        StructTBase("0.1 мкс"),
        StructTBase("0.2 мкс"),
        StructTBase("0.5 мкс"),
        StructTBase("1 мкс"),
        StructTBase("2 мкс"),
        StructTBase("5 мкс"),
        StructTBase("10 мкс"),
        StructTBase("20 мкс"),
        StructTBase("50 мкс"),
        StructTBase("0.1 мс"),
        StructTBase("0.2 мс"),
        StructTBase("0.5 мс"),
        StructTBase("1 мс"),
        StructTBase("2 мс"),
        StructTBase("5 мс"),
        StructTBase("10 мс"),
        StructTBase("20 мс"),
        StructTBase("50 мс"),
        StructTBase("0.1 с"),
        StructTBase("0.2 с"),
        StructTBase("0.5 с"),
        StructTBase("1 с"),
        StructTBase("2 с"),
        StructTBase("5 с"),
        StructTBase("10 с")
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
        BIN_U8(00000000),  // -V2501  // 200ns   2       100MHz
        BIN_U8(00100000),  // -V2501  // 500ns   5       40MHz
        BIN_U8(00100001),  // -V2501  // 1us     10      20MHz
        BIN_U8(00100010),  // -V2501  // 2us     20      10MHz
        BIN_U8(00100101),  // -V2501  // 5us     50      4MHz
        BIN_U8(00100110),  // -V2501  // 10us    100     2MHz
        BIN_U8(00100111),  // -V2501  // 20us    200     1MHz
        BIN_U8(00101001),  // -V2501  // 50us    500     400kHz
        BIN_U8(00101010),  // -V2501  // 100us   1k      200kHz
        BIN_U8(00101011),  // -V2501  // 200us   2k      100kHz
        BIN_U8(00101101),  // -V2501  // 500us   5k      40kHz
        BIN_U8(00101110),  // -V2501  // 1ms     10k     20kHz
        BIN_U8(00101111),  // -V2501  // 2ms     20k     10kHz
        BIN_U8(00110001),  // -V2501  // 5ms     50k     4kHz
        BIN_U8(00110010),  // -V2501  // 10ms    100k    2kHz
        BIN_U8(00110011),  // -V2501  // 20ms    200k    1kHz
        BIN_U8(00110101),  // -V2501  // 50ms    500k    400Hz
        BIN_U8(00110110),  // -V2501  // 100ms   1M      200Hz
        BIN_U8(00110111),  // -V2501  // 200ms   2M      100Hz
        BIN_U8(00111001),  // -V2501  // 500ms   5M      40Hz
        BIN_U8(00111010),  // -V2501  // 1s      10M     20Hz
        BIN_U8(00111011),  // -V2501  // 2s      20M     10Hz
        BIN_U8(00111101),  // -V2501  // 5s      50M     4Hz
        BIN_U8(00111110)   // -V2501  // 10s     100M    2Hz
    };

    Osci::ClearDataRand();

    HAL_BUS::FPGA::Write8(WR::TBASE, values[S_TIME_BASE]);

    FPGA::LoadRegUPR();

    TShift::Load();

    Bandwidth::Load(ChA);
    Bandwidth::Load(ChB);

    MessageMgr::OsciSettingsEffectOnAverageChanged();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TBase);
}


void Range::LoadBoth()
{
    uint16 val = (uint16)(ValueForRange(ChB) + (ValueForRange(ChA) << 8));

    Osci::CircuitController::Write(PIN_SPI3_CS2, val);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    Osci::CircuitController::Write(PIN_SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

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

    Bandwidth::Load(ChA);
    Bandwidth::Load(ChB);

    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        RShift::Load(ChB);
        RShift::Load(ChA);
    }
    else
    {
        RShift::Load(ChA);
        RShift::Load(ChB);
    }

    Osci::Restart();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


pcharc Range::ToString(Ch::E ch, Divider::E divider)
{
    return ranges[S_RANGE(ch)][divider].name;
}


pcharc Range::ToString(Range::E range, Divider::E _divider)
{
    return ranges[range][_divider].name;
}


static uint8 ValueForRange(Ch::E ch) // -V2506
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

    return (uint8)(values[S_RANGE(ch)][ch] | datas[couple]);
}


void TrigLevel::Find()
{
    Ch::E ch = S_TRIG_SOURCE;

    if (DS && ENABLED_DS(ch))
    {
        const uint8 *data = IN(ch);

        int numBytes = DS->BytesInChannel();

        uint8 max = Math::MaxFromArray(data, 0, (int)(numBytes) - 1);
        uint8 min = Math::MinFromArray(data, 0, (int)(numBytes) - 1);

        int deltaValue = (int)(VALUE::AVE) - (max + min) / 2;

        int deltaRShift = S_RSHIFT(ch);

        float k = 200 / 125.0F;     // Этот коэффициент получается так, что на верхей границе экрана лежит 125-я точка сигнала от центра экрана (нулевого значение),
                                    // а маркер в этой точке смещён на 200 единиц относительно цента экрана

        float additionShift = deltaValue + deltaRShift / k;     // Итоговое смщение, которое нужно добавить к TrigLev::Zero

        TrigLevel::Set(ch, (int16)(-additionShift * k + 0.5F));
    }
}


void RShift::DrawBoth()
{
    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        RShift::Draw(ChB);
        RShift::Draw(ChA);
    }
    else
    {
        RShift::Draw(ChA);
        RShift::Draw(ChB);
    }
}


pcharc Ch::Name(Ch::E ch)
{
    static pcharc names[Ch::Count] =
    {
        "Канал 1",
        "Канал 2"
    };

    return names[ch];
}


void TrigLevel::Load()
{
    // \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)(HARDWARE_ZERO - S_TRIG_LEVEL_SOURCE);

    Osci::CircuitController::Write(PIN_SPI3_CS1, (uint16)(0xa000 | (value << 2)));

    Osci::Restart();
}


void TrigLevel::Change(int16 delta)
{
    Math::AdditionThisLimitation(&S_TRIG_LEVEL_SOURCE, TrigLevel::STEP * delta, TrigLevel::MIN, TrigLevel::MAX);

    Load();

    DisplayOsci::DrawingValueParameter::Enable(DisplayOsci::DrawingValueParameter::TrigLevel);
}


void TrigLevel::Set(Ch::E ch, int16 newLevel)
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


void RShift::Draw(Ch::E ch)
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
        SymbolUGO(SymbolUGO::TRIGLEV_LOWER).Draw(x + 1, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 3, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x += 1;
    }
    else if (y < Grid::Top())
    {
        SymbolUGO(SymbolUGO::TRIGLEV_ABOVE).Draw(x + 1, Grid::Top() + 2);
        Pixel().Draw(x + 3, Grid::Top() + 2);
        y = Grid::Top() + 7;
        x += 1;
    }
    else
    {
        SymbolUGO(SymbolUGO::RSHIFT_NORMAL).Draw(x - 8, y - 8);
    }

    Char((ch == ChA) ? (uint8)('1') : (uint8)('2'), TypeFont::Small).Draw(x - 6, y - 6, Color::BACK);
}


void TrigLevel::Draw()
{
    Ch::E ch = S_TRIG_SOURCE;

    float scale = 1.0F / ((MAX - MIN) / 2.4F / Grid::Height());

    int y = Grid::ChannelCenterHeight() - (int)((S_TRIG_LEVEL(ch) + S_RSHIFT(ch)) * scale);

    int x = Grid::Right();
    int xSymbol = Grid::Right() + 9;

    Color::Trig().SetAsCurrent();

    if (y > Grid::ChannelBottom())
    {
        SymbolUGO(SymbolUGO::TRIGLEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        xSymbol--;
    }
    else if (y < Grid::Top())
    {
        SymbolUGO(SymbolUGO::TRIGLEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Pixel().Draw(x + 5, Grid::Top() + 2);
        y = Grid::Top() + 7;
        xSymbol--;
    }
    else
    {
        SymbolUGO(SymbolUGO::TRIGLEV_NORMAL).Draw(x + 1, y - 8);
    }

    static const char symbols[2] = { '1', '2' };

    Char((uint8)symbols[ch], TypeFont::Small).Draw(xSymbol, y - 6, Color::BACK);
}


float RShift::ToAbs(int16 rShift, Range::E range)
{
    return (rShift * absStep[range]);
}


int16 RShift::ToRel(float rShiftAbs, Range::E range)
{
    int16 retValue = (int16)(rShiftAbs / absStep[range]);

    if (retValue < MIN)
    {
        retValue = MIN;
    }
    else if (retValue > MAX)
    {
        retValue = MAX;
    }

    return retValue;
}


bool RShift::ChangeMath(int delta)
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
    return absStep[tBase] * tShift;
}

void VALUE::PointsToVoltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = Range::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    int diff = (int)((MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F);
    float koeff = 1.0F / 20e3F;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff; //-V636
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
    int relValue = (int)((voltage + Range::MaxVoltageOnScreen(range) + RShift::ToAbs(rShift, range)) / voltsInPoint[range] + MIN);
    ::Math::Limitation<int>(&relValue, 0, 255);
    return (uint8)(relValue);
}


float VALUE::ToVoltage(uint8 value, Range::E range, int16 rShift)
{
    uint8 delta = (uint8)(value - MIN);

    float rShiftAbs = RShift::ToAbs(rShift, range);

    float maxVoltage = Range::MaxVoltageOnScreen(range);

    return delta * voltsInPoint[range] - maxVoltage - rShiftAbs;
}


void VALUE::PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points)
{
    float maxVoltOnScreen = Range::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    float voltInPixel = 1.0F / (voltsInPoint[range] / ((MAX - MIN) / 200.0F));

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + MIN;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);

        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }

        points[i] = (uint8)(value);
    }
}


void Range::Set(Ch::E ch, E range)
{
    set.disp.SetLastAffectedChannel(ch);

    S_RANGE(ch) = range;

    LoadBoth();

    DisplayOsci::DrawingValueParameter::Enable((ch == ChA) ? DisplayOsci::DrawingValueParameter::RangeA : DisplayOsci::DrawingValueParameter::RangeB);
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
    SymbolUGO(SymbolUGO::TSHIFT_NORMAL_BACK).Draw(x - 3, y, Color::BACK);
    SymbolUGO(SymbolUGO::TSHIFT_NORMAL).Draw(x - 3, y, Color::FILL);
}


void TShift::DrawLeft()
{
    SymbolUGO(SymbolUGO::TRIGLEV_NORMAL).Draw(Grid::Left(), Grid::Top(), Color::BACK);
    SymbolUGO(SymbolUGO::TSHIFT_LEFT).Draw(Grid::Left(), Grid::Top(), Color::FILL);
}

void TShift::DrawRight()
{
    SymbolUGO(SymbolUGO::RSHIFT_NORMAL).Draw(Grid::Right() - 7, Grid::Top(), Color::BACK);
    SymbolUGO(SymbolUGO::TSHIFT_RIGHT).Draw(Grid::Right() - 7, Grid::Top(), Color::FILL);
}
