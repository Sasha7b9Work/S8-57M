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
#include <cstring>
#include <cstdio>


// ��������� ��� �������� ����������, ����������� ��� ������ � ������ �������������
struct StructReadRand
{
    StructReadRand() : step(0), posFirst(0) { }
    int step;       // ��� ����� �������
    int posFirst;   // ������� ������ ��������� �����
};


// ��������� ��� ������ �� ����������� ����� � �������������
struct RandShift
{
    // ���������� ������, ����������� ��� ������ ������ � ������� �������������.
    // ���� Tsm == 0, �� ��������� ����� �������������� �� ��� ������ ������, � ��� ����������� ���������.
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
    // ����� �������� ��� ���������� � ��� ���������� �������� �� ���� ���������� � ������� Calculate
    MinMax2 m;
    // ����������� �������� �����
    void RecalculateGates();

    void CalculateWithoutGates(uint16 *min, uint16 *max);

    bool Calculate(uint16 value, uint16 *min, uint16 *max);
};


static Gates gates;             // "������" �������������

static void UpdateFPGA();

// � ����������� �� ��������� ����� ���������� ������ ������ ������ � ���������� ���� ������������� ���������� ������� ����� ����������
static bool ProcessFlagReady();


void Osci::Init()
{
    Stop();

    RAM::Init();
    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift::Load(ChanA);
    RShift::Load(ChanB);
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
    addrRead = 0xffff;

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


void Osci::Update() //-V2506
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
    int number = (OSCI_IN_MODE_RANDOMIZER && !SampleType::IsReal()) ? TBase::DeltaPoint() : 1;

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
        if(S_OSCI_AVERAGING_IS_ENABLED)               // ���� �������� ����������
        {
            DataSettings *last = RAM::Get(0);
            DataSettings *prev = RAM::Get(1);

            if(prev && last)
            {
                if(last->IsEquals(*prev))
                {
                    if(ENABLED_A(last))
                    {
                        AveragerOsci::Process(ChanA, last->dataA, last->BytesInChannel());
                    }
                    if(ENABLED_B(last))
                    {
                        AveragerOsci::Process(ChanB, last->dataB, last->BytesInChannel());
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


void Osci::InputController::Init()
{
    HAL_PIO::Init(PIN_SPI3_SCK, HMode::Output_PP, HPull::Down);     // SPI3_SCK
    HAL_PIO::Init(PIN_SPI3_DAT, HMode::Output_PP, HPull::Down);     // SPI3_DAT
    HAL_PIO::Init(PIN_SPI3_CS1, HMode::Output_PP, HPull::Down);     // SPI3_CS1
    HAL_PIO::Init(PIN_SPI3_CS2, HMode::Output_PP, HPull::Down);     // SPI3_CS2
    HAL_PIO::Init(PIN_A1, HMode::Output_PP, HPull::Down);           // A1
    HAL_PIO::Init(PIN_A2, HMode::Output_PP, HPull::Down);           // A2
    HAL_PIO::Init(PIN_A3, HMode::Output_PP, HPull::Down);           // A3
    HAL_PIO::Init(PIN_A4, HMode::Output_PP, HPull::Down);           // A4
    HAL_PIO::Init(PIN_LF1, HMode::Output_PP, HPull::Down);          // LF1 ����������� ������
    HAL_PIO::Init(PIN_LF2, HMode::Output_PP, HPull::Down);          // LF2 ����������� ������
    HAL_PIO::Init(PIN_LF3, HMode::Output_PP, HPull::Down);          // A1S
    HAL_PIO::Init(PIN_A0S, HMode::Output_PP, HPull::Down);          // A0S
    HAL_PIO::Init(PIN_LFS, HMode::Output_PP, HPull::Down);          // LFS
}

void Osci::InputController::Write(HPort::E portCS, uint16 pinCS, uint16 value)
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
    else if(portCS == PORT_SPI3_CS2) //-V2516
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

        std::memset(ds->Data(ChanA), VALUE::NONE, static_cast<uint>(ds->PointsInChannel()));
        std::memset(ds->Data(ChanB), VALUE::NONE, static_cast<uint>(ds->PointsInChannel()));

        std::memset(IntRAM::DataRand(ChanA), VALUE::NONE, static_cast<uint>(ds->PointsInChannel()));
    }
}


void Osci::ReadData()
{
    Osci::Stop();

    DataSettings *ds = RAM::PrepareForNewData();

    if(ReadDataChannel(ChanA, ds->dataA))
    {
        if(ReadDataChannel(ChanB, ds->dataB))
        {
            if (SampleType::IsReal())
            {
                InterpolatorSinX_X::Run(ds);
            }
        }
    }
}


bool Osci::ReadDataChannelRand(uint8 *addr, uint8 *data) //-V2506
{
    ShiftPoint Tsm = gates.CalculateShiftPoint();

    if(Tsm.type == ShiftPoint::FAIL)
    {
        return false;
    }

    StructReadRand infoRead = RandShift::GetInfoForReadRand(Tsm, addr);

    int step = infoRead.step;

    uint8 *dataWrite = data + infoRead.posFirst;                                // ���� ������� ������ ��������� ����� //-V2563

    uint8 *interpolated = IntRAM::DataRand(ChanA) + infoRead.posFirst; //-V2563

    uint8 *last = data + ENumPointsFPGA::PointsInChannel(); //-V2563

    HAL_BUS::FPGA::SetAddrData(addr);

    if(S_OSCI_AVERAGING_IS_ENABLED)
    {
        uint8 *dataPointer = &data[infoRead.posFirst];              // ��������� � ���������� ������� //-V2563

        while(dataWrite < last)
        {
            *dataWrite = HAL_BUS::FPGA::ReadA0();
            *dataPointer = *dataWrite;
            *interpolated = *dataWrite;
            
            dataWrite += step; //-V2563
            dataPointer += step; //-V2563
            interpolated += step; //-V2563
        }
    }
    else
    {
        while(dataWrite < last)
        {
            *dataWrite = HAL_BUS::FPGA::ReadA0();
            *interpolated = *dataWrite;

            dataWrite += step; //-V2563
            interpolated += step; //-V2563

        }
    }

    return true;
}


ShiftPoint Gates::CalculateShiftPoint() //-V2506
{
    ShiftPoint result(ShiftPoint::READED, 0);

    uint16 min = 0;
    uint16 max = 0;

    uint16 valueADC = HAL_ADC3::ValueRandomizer();

    if(!Calculate(valueADC, &min, &max))
    {
        result.type = ShiftPoint::FAIL;
        return result;
    }

    if(((valueADC > max - NRST_ENUM_GATE_MAX * 10) || (valueADC < min + NRST_ENUM_GATE_MIN * 10)) && (S_TIME_BASE > TBase::_5ns))
    {
        result.type = ShiftPoint::FAIL;
        return result;
    }

    float tin = static_cast<float>(valueADC - min) / (max - min); //-V2564

    result.shift = static_cast<int>(tin * TBase::DeltaPoint()); //-V2564

    if(result.shift < 0)
    {
        result.shift = 0;
    }
    else if(result.shift >= TBase::DeltaPoint()) //-V2516
    {
        result.shift = TBase::DeltaPoint() - 1;
    }

    if(((valueADC > max - NRST_ENUM_GATE_MAX * 10) || (valueADC < min + NRST_ENUM_GATE_MIN * 10)) && (S_TIME_BASE < TBase::_10ns))
    {
        result.type = ShiftPoint::FAIL;
    }

    return result;
}


StructReadRand RandShift::GetInfoForReadRand(ShiftPoint Tsm, const uint8 *address)
{
    static const int add[] =
    {
        150,    // 2 ns
        75,     // 5 ns
        30,     // 10 ns
        15,     // 20 ns
        3       // 50 ns
    };

    StructReadRand structRand;

    if(Tsm.type != ShiftPoint::FAIL)
    {
        int step = TBase::DeltaPoint();

        structRand.step = step;

        int addShift = static_cast<int>(S_TIME_SHIFT % step);

        if (addShift < 0)
        {
            addShift += step;
        }

        int index = Tsm.shift - addShift - step - add[S_TIME_BASE];

        while(index < 0)
        {
            index += step;

            volatile uint8 d = *address;    // ��� ������� ������� ������ - ��������� � ����������� �������������� �����
            d = d;                          // 
        }

        structRand.posFirst = index;
    }

    return structRand;
}


bool Gates::Calculate(uint16 value, uint16 *min, uint16 *max)  //-V2506
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

    if(minGate == 0.0F) //-V550 //-V2550
    {
        *min = m.Min();
        *max = m.Max();
        if(m.Count() < numberMeasuresForGates)
        {
            return true;
        }
        minGate = m.Min(); //-V2564
        maxGate = m.Max(); //-V2564
        m.Reset();
    }

    if(m.Count() >= numberMeasuresForGates)
    {
        RecalculateGates();

        m.Reset();
    }

    *min = static_cast<uint16>(minGate);
    *max = static_cast<uint16>(maxGate);

    return (value >= *min) && (value <= *max);
}


void Gates::RecalculateGates()
{
    minGate = 0.8F * minGate + m.Min() * 0.2F; //-V2564
    maxGate = 0.8F * maxGate + m.Max() * 0.2F; //-V2564

    if(S_DBG_SHOW_RAND_GATES)
    {
        static uint timePrev = 0;
        LOG_WRITE("����� ������ %d %d  ����� %d", static_cast<uint16>(minGate), static_cast<uint16>(maxGate), (TIME_MS - timePrev) / 1000);
        timePrev = TIME_MS;
    }
}


void Gates::CalculateWithoutGates(uint16 *min, uint16 *max)
{
    if(minGate == 0.0F) //-V550 //-V2550
    {
        *min = m.Min();
        *max = m.Max();
    }
    else
    {
        *min = static_cast<uint16>(minGate);
        *max = static_cast<uint16>(maxGate);
    }
}


OsciStateWork::E OsciStateWork::Current() //-V2506
{
    if (!Osci::IsRunning())
    {
        return OsciStateWork::Stopped;
    }

    return triggered ? OsciStateWork::Triggered : OsciStateWork::Awaiting;
}


void Osci::SendDataToSCPI()
{
    SendDataToSCPI(Chan::A);
    SendDataToSCPI(Chan::B);
}


void Osci::SendDataToSCPI(Chan::E ch) //-V2506
{
    if (!SCPI::Sender::osci[ch] || ENABLE_CH_DS(ch) == 0)
    {
        return;
    }

    SCPI::Sender::osci[ch] = false;

    int numBytes = DS->BytesInChannel();

    char buffer[100];

    uint8 *data = const_cast<DataSettings *>(DS)->Data(ch); //-V2567

    SCPI::SendData(ch == Chan::A ? "1: " : "2: ");

    for (int i = 0; i < numBytes - 1; i++)
    {
        std::sprintf(buffer, "%d ", data[i]); //-V2563
        SCPI::SendData(buffer);
    }

    std::sprintf(buffer, "%d", data[numBytes - 1]); //-V2563
    SCPI::SendAnswer(buffer);
}
