// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_METER_ENABLED    (set.freq.enabled)


struct SettingsFreqMeter
{
    bool                         enabled;          // Включён ли режим частотомера
    FreqMeter::ModeMeasure::E    modeMeasure;      // Режим измерения - период или частота
    FreqMeter::TimeCounting::E   timeCounting;     // Время счёта частоты.
    FreqMeter::FreqClc::E        freqClc;          // Частота заполнения.
    FreqMeter::NumberPeriods::E  numberPeriods;    // Количество периодов.
};



namespace PageFreqMeter
{
    extern const Page *const self;

    void Init();
};
