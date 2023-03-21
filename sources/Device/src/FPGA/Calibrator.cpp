// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "FPGA/Calibrator.h"
#include "FPGA/FPGA.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cmath>
#include <cstring>
#include <cstdio>


namespace Calibrator
{
    // Откалибровать канал Ch
    static bool CalibrateChannel(Ch::E ch);

    // Балансировать канал на одном диапазоне
    static void BalanceRange(Ch::E ch, Range::E range);

    // "Растянуть" канал
    static bool StretchChannel(Ch::E ch);

    // Найти коэффициент растяжки канала
    static float FindStretchChannel(Ch::E ch);
}


void Calibrator::Calibrate()
{
    Settings old = set;

    setNRST.ResetExtraStretch();
    setNRST.SetExtraShift(0);

    if (!CalibrateChannel(ChA))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 1 не прошла. Нажмите любую кнопку.", true);
    }
    
    if (!CalibrateChannel(ChB))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 2 не прошла. Нажмите любую кнопку.", true);
    }

    Display::Message::ShowAndWaitKey("Калибровка завершена. Нажмите любую кнопку.", true);

    set = old;

    Osci::Init();
}


bool Calibrator::CalibrateChannel(Ch::E ch)
{
    Display::Message::ShowAndWaitKey(ch == ChA ?  "Подключите встроенный калибратор ко входу 1 и нажмите любую кнопку" :
                                                  "Подключите встроенный калибратор ко входу 2 и нажмите любую кнопку", true);

    Display::Message::Show(ch == ChA ? "Калибрую канал 1" : "Калибрую канал 2", true);

    bool result = Calibrator::BalanceChannel(ch, false) && StretchChannel(ch);

    Display::Message::Hide();

    return result;
}


bool Calibrator::BalanceChannel(Ch::E ch, bool showHint)
{
    Settings old = set;

    SettingsNRST oldNRST = setNRST;

    setNRST.SetExtraShift(0);

    static const pcharc messages[Ch::Count] =
    {
        "Балансирую канал 1",
        "Балансирую канал 2"
    };

    if (showHint)
    {
        Display::Message::Show(messages[ch], true);
    }

    ModeCouple::Set(ch, ModeCouple::GND);

    TBase::Set(TBase::_100ms);

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceRange(ch, (Range::E)range);
    }

    std::memcpy(&oldNRST.exShift[ch][0], &NRST_EX_SHIFT(ch, 0), sizeof(NRST_EX_SHIFT(ch, 0)) * Range::Count);

    setNRST = oldNRST;
    set = old;

    Osci::Init();

    if (showHint)
    {
        Display::Message::Hide();
    }

    return true;
}


void Calibrator::BalanceRange(Ch::E ch, Range::E range)
{
    Osci::Stop();

    Range::Set(ch, range);

    RShift::Set(ch, 0);

    for (int i = 0; i < 2 * 10000000; i++)
    {
        i++;
    }

    Osci::Start(false);

    float sum = 0;

    int numPoints = 0;

    uint8 *addr = ((ch == ChA) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < 100)
    {
        if (!HAL_BUS::Panel::InInteraction())
        {
            if (HAL_PIO::Read(PIN_P2P))
            {
                HAL_BUS::FPGA::SetAddrData(addr);
                sum += HAL_BUS::FPGA::ReadA0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        NRST_EX_SHIFT(ch, range) = (int8)(delta * 400.0F / 125.0F + 0.5F);
    }
    else
    {
        NRST_EX_SHIFT(ch, range) = (int8)(delta * 400.0F / 125.0F - 0.5F);
    }
}


bool Calibrator::StretchChannel(Ch::E ch)
{
    SettingsNRST old = setNRST;

    setNRST.ResetExtraStretch();

    ModeCouple::Set(ch, ModeCouple::AC);
    RShift::Set(ch, 0);
    Range::Set(ch, Range::_500mV);
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigLevel::Set(ch, 0);


    float k = FindStretchChannel(ch);

    if (k > 0.0F)
    {
        old.exStretch[ch] = k;
    }

    setNRST = old;

    Osci::Init();

    return (k > 0.0F);
}


float Calibrator::FindStretchChannel(Ch::E ch)
{
#define NUM_POINTS 300
    
    Timer::PauseOnTime(500);

    FPGA::GiveStart((uint16)(~(1)), (uint16)(~(NUM_POINTS + 100)));
    
    FPGA::Flag::Clear();

    while(!FPGA::Flag::Pred())
    {
        FPGA::Flag::Read();
    }

    FPGA::ForcedStart();

    do 
    {
        FPGA::Flag::Read();

    } while (!FPGA::Flag::DataReady());

    uint16 addrRead = (uint16)(Osci::ReadLastRecord(ch) - NUM_POINTS - 50);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, addrRead);
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = (ch == ChA) ? RD::DATA_A : RD::DATA_B;
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    volatile uint8 data = *a0;
    data = *a1; //-V519

    int sumMIN = 0;
    int sumMAX = 0;
    int numMIN = 0;
    int numMAX = 0;

    for(uint i = 0; i < NUM_POINTS; i++)
    {
        data = *a1;

        if(data < VALUE::MIN + 64)
        {
            sumMIN += data;
            numMIN++;
        }
        else if(data > VALUE::MAX - 64)
        {
            sumMAX += data;
            numMAX++;
        }
        else
        {
            return -1.0F;
        }
    }

    float patternDelta = (VALUE::MAX - VALUE::MIN) / 10.0F * 8.0F;    // Образцоввая разница между минимальным и максимальным значениями - ровно на 8 клеток из десяти

    float min = (float)sumMIN / numMIN;
    float max = (float)sumMAX / numMAX;

    return patternDelta / (max - min);
}
