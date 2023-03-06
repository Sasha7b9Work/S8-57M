// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_METER_ENABLED    (set.freq.enabled)


struct SettingsFreqMeter
{
    bool                         enabled;          // ������� �� ����� �����������
    FreqMeter::ModeMeasure::E    modeMeasure;      // ����� ��������� - ������ ��� �������
    FreqMeter::TimeCounting::E   timeCounting;     // ����� ����� �������.
    FreqMeter::FreqClc::E        freqClc;          // ������� ����������.
    FreqMeter::NumberPeriods::E  numberPeriods;    // ���������� ��������.
};



namespace PageFreqMeter
{
    extern const Page *const self;

    void Init();
};
