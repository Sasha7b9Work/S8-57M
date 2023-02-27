// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "globals.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Math/OsciMath.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Utils/Buffer.h"
#include <cstring>
#include <cstdio>


namespace Osci
{
    static void SendDataToSCPI(Ch::E ch);

    // Читать данные канала в памяить data
    bool ReadDataChannel(Ch::E ch, uint8 *data);

    bool ReadDataChannelRand(uint8 *address, uint8 *data);


    namespace AddrRead
    {
        // Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
        static uint16 addrRead = 0xffff;

        uint16 Get() { return addrRead; }

        void Set(uint16 addr) { addrRead = addr; }

        void Reset() { addrRead = 0xffff; }
    }
}


// Структура для хранения информации, необходимой для чтения в режиме рандомизатора
struct StructReadRand
{
    StructReadRand() : step(0), posFirst(0) { }
    int step;       // Шаг между точками
    int posFirst;   // Позиция первой считанной точки

    // Возвращает данные, необходимые для чтения даннхы в режмиме рандомизатора.
    // Если Tsm == 0, то структура будет использоваться не для чтения данных, а для правильного усредения.
    static StructReadRand GetInfoForReadRand(ShiftPoint Tsm, const uint8 *address = nullptr);
};


struct Gates
{
    Gates() : minGate(0.0F), maxGate(0.0F) { }

    ShiftPoint CalculateShiftPoint();

private:
    static const int numberMeasuresForGates = 10000;
    static const uint TIME_WAIT = 3000;
    float minGate;
    float maxGate;
    // Здесь хранятся два наименьших и два наибольших значения из всех подаваемых в функцию Calculate
    MinMax2 m;
    // Пересчитать значения ворот
    void RecalculateGates();

    void CalculateWithoutGates(uint16 *min, uint16 *max);

    bool Calculate(uint16 value, uint16 *min, uint16 *max);
};


static Gates gates;             // "Ворота" рандомизатора

static void UpdateFPGA();

// В зависимости от состояния флага готовности данных читает данные и возвращает флаг необходимости остановить процесс сбора информации
static bool ProcessFlagReady();


void Osci::Init()
{
    Stop();

    RAM::Init();
    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift::Load(ChA);
    RShift::Load(ChB);
    TrigInput::Load();
    TrigLevel::Load();
    TrigPolarity().Load();
    TBase::Set();
    TShift::Load();
    FPGA::LoadCalibratorMode();
    HAL_PIO::Init(PIN_P2P, HMode::Input, HPull::Up);
    ChangedTrigStartMode();
    Osci::Start(true);
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start(bool)
{
    FPGA::forcedStart = false;

    AddrRead::Reset();

    Roller::Prepare();

    FPGA::GiveStart(FPGA::pred, FPGA::post);

    FPGA::isRunning = true;
}


void Osci::Stop()
{
    FPGA::isRunning = false;
}


void Osci::Restart()
{
    if(FPGA::IsRunning())
    {
        Stop();
        Start(true);
    }

    ClearDataRand();
}


void Osci::Update()
{
    if(!Device::InModeOsci())
    {
        return;
    }

    if(FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromRAM();

    Osci::SendDataToSCPI();

    AutoMeasurements::SetData();
}


static void UpdateFPGA()
{
    int number = (OSCI_IN_MODE_RANDOMIZER && !SampleType::IsReal()) ? TBase::DeltaPointRand() : 1;

    RAM::NewFrameForRandomize();

    uint lastID = 0;

    DataSettings *ds = RAM::Get();

    if(ds)
    {
        lastID = ds->id;
    }

    for(int i = 0; i < number; i++)
    {
        FPGA::Flag::Read();

        Osci::ProcessFlagPred();

        if(ProcessFlagReady())
        {
            Osci::Stop();
            break;
        }
    }

    if(OSCI_IN_MODE_RANDOMIZER && !SampleType::IsReal())
    {
        InterpolatorLinear::Run(RAM::Get());
    }

    ds = RAM::Get();

    if(ds && ds->id != lastID)
    {
        if(S_OSCI_AVERAGING_IS_ENABLED)               // Если включено усреднение
        {
            DataSettings *last = RAM::Get(0);
            DataSettings *prev = RAM::Get(1);

            if(prev && last)
            {
                if(last->IsEquals(*prev))
                {
                    if(ENABLED_A(last))
                    {
                        AveragerOsci::Process(ChA, last->ch_a, last->BytesInChannel());
                    }
                    if(ENABLED_B(last))
                    {
                        AveragerOsci::Process(ChB, last->ch_b, last->BytesInChannel());
                    }
                }
            }
        }
    }
}


static bool ProcessFlagReady()
{
    bool needStop = false;

    if(FPGA::Flag::DataReady())
    {
        Timer::PauseOnTicks(5 * 90 * 20);

        Osci::ReadData();

        if(S_TRIG_START_MODE_IS_SINGLE)
        {
            needStop = true;
            Trig::pulse = false;
        }
        else
        {
            Timer::PauseOnTicks(5 * 90 * 20);
            Osci::Start(false);
        }
    }

    return needStop;
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::ProcessFlagPred()
{
    if(FPGA::Flag::Pred() && !FPGA::forcedStart)
    {
        if(!OSCI_IN_MODE_RANDOMIZER && S_TRIG_START_MODE_IS_AUTO && FPGA::Flag::HoldOff())
        {
            FPGA::ForcedStart();
        }
    }
}


void Osci::OnChangedLengthMemoryChannel()
{
    FPGA::LoadRegUPR();
    FPGA::Reset();
    DisplayOsci::ShiftInMemory::OnChangeTPos();
    FPGA::Reset();
    TShift::Set(S_TIME_SHIFT);
    FPGA::Reset();
    RAM::Init();
}


void Osci::OnPressButtonStart()
{
    if (IsRunning())
    {
        Stop();
        OsciStateWork::triggered = false;
    }
    else
    {
        Start(true);
    }
}


void Osci::ChangedTrigStartMode()
{
    Stop();

    if(!S_TRIG_START_MODE_IS_SINGLE)
    {
        Start(true);
    }
}


void Osci::CircuitController::Init()
{
    HAL_PIO::Init(PIN_SPI3_SCK, HMode::Output_PP, HPull::Down);     // SPI3_SCK
    HAL_PIO::Init(PIN_SPI3_DAT, HMode::Output_PP, HPull::Down);     // SPI3_DAT
    HAL_PIO::Init(PIN_SPI3_CS1, HMode::Output_PP, HPull::Down);     // SPI3_CS1
    HAL_PIO::Init(PIN_SPI3_CS2, HMode::Output_PP, HPull::Down);     // SPI3_CS2
    HAL_PIO::Init(PIN_A1, HMode::Output_PP, HPull::Down);           // A1
    HAL_PIO::Init(PIN_A2, HMode::Output_PP, HPull::Down);           // A2
    HAL_PIO::Init(PIN_A3, HMode::Output_PP, HPull::Down);           // A3
    HAL_PIO::Init(PIN_A4, HMode::Output_PP, HPull::Down);           // A4
    HAL_PIO::Init(PIN_LF1, HMode::Output_PP, HPull::Down);          // LF1 Ограничение полосы
    HAL_PIO::Init(PIN_LF2, HMode::Output_PP, HPull::Down);          // LF2 Ограничение полосы
    HAL_PIO::Init(PIN_LF3, HMode::Output_PP, HPull::Down);          // A1S
    HAL_PIO::Init(PIN_A0S, HMode::Output_PP, HPull::Down);          // A0S
    HAL_PIO::Init(PIN_LFS, HMode::Output_PP, HPull::Down);          // LFS
}

void Osci::CircuitController::Write(HPort::E portCS, uint16 pinCS, uint16 value)
{
    HAL_PIO::Reset(portCS, pinCS);

    if(portCS == PORT_SPI3_CS1)
    {
        for(int i = 15; i >= 0; --i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else if(portCS == PORT_SPI3_CS2)
    {
        for(int i = 0; i < 16; ++i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }

    HAL_PIO::Set(portCS, pinCS);
}


void Osci::ClearDataRand()
{
    if(OSCI_IN_MODE_RANDOMIZER)
    {
        DataSettings *ds = RAM::Get();
        ds->Fill();

        std::memset(ds->Data(ChA), VALUE::NONE, (uint)(ds->PointsInChannel()));
        std::memset(ds->Data(ChB), VALUE::NONE, (uint)(ds->PointsInChannel()));

        std::memset(IntRAM::DataRand(ChA), VALUE::NONE, (uint)(ds->PointsInChannel()));
    }
}


void Osci::ReadData()
{
    Osci::Stop();

    DataSettings &ds = *RAM::PrepareForNewData();

    if(ReadDataChannel(ChA, ds.ch_a))
    {
        if(ReadDataChannel(ChB, ds.ch_b))
        {
            if (SampleType::IsReal())
            {
                InterpolatorSinX_X::Run(ds);
            }
        }
    }
}


bool Osci::ReadDataChannelRand(uint8 *addr, uint8 *out)
{
    ShiftPoint shift = gates.CalculateShiftPoint();

    if(!shift.IsValid())
    {
        return false;
    }

    LOG_WRITE("shift = %d", shift.shift);

    StructReadRand iread = StructReadRand::GetInfoForReadRand(shift, addr);

    int step = iread.step;

    uint8 *wr = out + iread.posFirst;                               // Сюда запишем первую считанную точку

    uint8 *ipol = IntRAM::DataRand(ChA) + iread.posFirst;            // interpolated

    uint8 *last = out + ENumPointsFPGA::PointsInChannel();

    HAL_BUS::FPGA::SetAddrData(addr);

    if(S_OSCI_AVERAGING_IS_ENABLED)
    {
        uint8 *data = &out[iread.posFirst];                  // Указатель в переданном массиве

        while(wr < last)
        {
            *wr = HAL_BUS::FPGA::ReadA0();
            *data = *wr;
            *ipol = *wr;
            
            wr += step;
            data += step;
            ipol += step;
        }
    }
    else
    {
        while(wr < last)
        {
            *wr = HAL_BUS::FPGA::ReadA0();
            *ipol = *wr;

            wr += step;
            ipol += step;
        }
    }

    return true;
}


ShiftPoint Gates::CalculateShiftPoint()
{
    ShiftPoint result;

    uint16 min = 0;
    uint16 max = 0;

    uint16 valueADC = HAL_ADC3::ValueRandomizer();

    if(!Calculate(valueADC, &min, &max))
    {
        return result;
    }

    if(((valueADC > max - NRST_ENUM_GATE_MAX * 10) || (valueADC < min + NRST_ENUM_GATE_MIN * 10)) && (S_TIME_BASE > TBase::_5ns))
    {
        return result;
    }

    float tin = (float)(valueADC - min) / (max - min);

    result.shift = (int)(tin * TBase::DeltaPointRand());

    if(result.shift < 0)
    {
        result.shift = 0;
    }
    else if(result.shift >= TBase::DeltaPointRand())
    {
        result.shift = TBase::DeltaPointRand() - 1;
    }

    if(((valueADC > max - NRST_ENUM_GATE_MAX * 10) || (valueADC < min + NRST_ENUM_GATE_MIN * 10)) && (S_TIME_BASE < TBase::_10ns))
    {
        result.shift = 0xffffffff;
    }

    return result;
}


StructReadRand StructReadRand::GetInfoForReadRand(ShiftPoint shift, const uint8 *address)
{
    static const int add[] =
    {
        300,    // 2 ns
        150,    // 5 ns
        60,     // 10 ns
        30,     // 20 ns
        6,      // 50 ns
        3       // 100 ns
    };

    StructReadRand structRand;

    if(shift.IsValid())
    {
        int step = TBase::DeltaPointRand();

        structRand.step = step;

        int addShift = (int)(S_TIME_SHIFT % step);

        if (addShift < 0)
        {
            addShift += step;
        }

        int index = shift.shift - addShift - step - add[S_TIME_BASE];

        while(index < 0)
        {
            index += step;

            volatile uint8 d = *address;    // Эти строчки удалять нельзя - считываем и отбрасываем неиспользуемые точки
            d = d;                          // 
        }

        structRand.posFirst = index;
    }

    return structRand;
}


bool Gates::Calculate(uint16 value, uint16 *min, uint16 *max) 
{
    if(value < 250 || value > 4000)
    {
        return false;
    }

    m.Add(value);

    if((TIME_MS > TIME_WAIT) && (BufferButtons::TimeAfterControlMS() < TIME_WAIT))
    {
        CalculateWithoutGates(min, max);

        return true;
    }

    if(minGate == 0.0F)
    {
        *min = m.Min();
        *max = m.Max();
        if(m.Count() < numberMeasuresForGates)
        {
            return true;
        }
        minGate = m.Min();
        maxGate = m.Max();
        m.Reset();
    }

    if(m.Count() >= numberMeasuresForGates)
    {
        RecalculateGates();

        m.Reset();
    }

    *min = (uint16)(minGate);
    *max = (uint16)(maxGate);

    return (value >= *min) && (value <= *max);
}


void Gates::RecalculateGates()
{
    minGate = 0.8F * minGate + m.Min() * 0.2F;
    maxGate = 0.8F * maxGate + m.Max() * 0.2F;

    if(S_DBG_SHOW_RAND_GATES)
    {
        static uint timePrev = 0;
        LOG_WRITE("Новые ворота %d %d  время %d", (uint16)(minGate), (uint16)(maxGate), (TIME_MS - timePrev) / 1000);
        timePrev = TIME_MS;
    }
}


void Gates::CalculateWithoutGates(uint16 *min, uint16 *max)
{
    if(minGate == 0.0F)
    {
        *min = m.Min();
        *max = m.Max();
    }
    else
    {
        *min = (uint16)(minGate);
        *max = (uint16)(maxGate);
    }
}


OsciStateWork::E OsciStateWork::Current()
{
    if (!Osci::IsRunning())
    {
        return OsciStateWork::Stopped;
    }

    return triggered ? OsciStateWork::Triggered : OsciStateWork::Awaiting;
}


void Osci::SendDataToSCPI()
{
    SendDataToSCPI(Ch::A);
    SendDataToSCPI(Ch::B);
}


void Osci::SendDataToSCPI(Ch::E ch)
{
    if (!SCPI::Sender::osci[ch] || ENABLE_CH_DS(ch) == 0)
    {
        return;
    }

    SCPI::Sender::osci[ch] = false;

    int numBytes = DS->BytesInChannel();

    char buffer[100];

    uint8 *data = ((DataSettings *)DS)->Data(ch);

    SCPI::SendData(ch == Ch::A ? "1: " : "2: ");

    for (int i = 0; i < numBytes - 1; i++)
    {
        std::sprintf(buffer, "%d ", data[i]);
        SCPI::SendData(buffer);
    }

    std::sprintf(buffer, "%d", data[numBytes - 1]);
    SCPI::SendAnswer(buffer);
}
