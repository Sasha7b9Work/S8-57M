// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_METER_ENABLED    (set.freq._enabled)


struct SettingsFreqMeter
{
    bool                         _enabled;          // ������� �� ����� �����������
    FreqMeter::ModeMeasure::E    _modeMeasure;      // ����� ��������� - ������ ��� �������
    FreqMeter::TimeCounting::E   _timeCounting;     // ����� ����� �������.
    FreqMeter::FreqClc::E        _freqClc;          // ������� ����������.
    FreqMeter::NumberPeriods::E  _numberPeriods;    // ���������� ��������.
};



namespace PageFreqMeter
{
    extern const Page *const self;

    void Init();
};
