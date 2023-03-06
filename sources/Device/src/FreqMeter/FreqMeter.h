// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Stack.h"


namespace FreqMeter
{
    // �������������.
    void Init();

    // ������� ��������� ��� �������� �������
    void LoadFreqSettings();

    // ������� �������� ��� �������� �������
    void LoadPeriodSettings();

    void Update();

    // ���������� ���������� �������� �������
    float GetFrequency();

    // ��� ������ ������� ����� ������� �������� ��� �������, ����� �������� ���������. ���� �� ����� �������� ���������, ��������, ������������� �������� GetFreq(), ����� ����� 0.0F
    void ClearMeasure();

    // ���������� true, ���� ���� ���������� �������� �������
    bool FrequencyIsFound();

    extern BitSet32 freqActual;             // ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    extern BitSet32 periodActual;           // ����� �������� ��������� �������������� �������� �������. ��� ������ � ������ �����������. 0 ��������, ��� �������� �������� �� ����
    extern uint     timeStartMeasureFreq;   // ����� ������ ��������� �������
    extern uint     timeStartMeasurePeriod; // ����� ������ ��������� �������
    extern uint     lastFreqRead;           // ��������� ����� ����������� ������ �������
    extern uint     lastPeriodRead;         // ��������� ����� ���������� ������ �������
    extern uint     lastFreqOver;           // ��������� ����� ������������ �������
    extern uint     lastPeriodOver;         // ��������� ����� ����������� �������

    namespace FPGA
    {
        void LoadSettings();
        void ResetCounterFreq();
        void ResetCounterPeriod();
        extern BitSet32 ReadCounterFreq();
        extern BitSet32 ReadCounterPeriod();
    };

    // ������� ����������� ��������� ��� ����� �������.
    struct FreqClc
    {
#define S_FREQ_FREQ_CLC     (set.freq.freqClc)

        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz,
            Count
        };
    };

    // ���������� ��������.
    struct NumberPeriods
    {
#define S_FREQ_NUMBER_PERIODS   (set.freq.numberPeriods)

        enum E
        {
            _1,
            _10,
            _100,
            _1k,
            _10k,
            _100k,
            Count
        };
    };

    // ��� ���������� - ������ ��� �������
    struct ModeMeasure
    {
#define S_FREQ_MODE_MEASURE                (set.freq.modeMeasure)
#define S_FREQ_MODE_MEASURE_IS_FREQUENCY   (S_FREQ_MODE_MEASURE == FreqMeter::ModeMeasure::Frequency)
#define S_FREQ_MODE_MEASURE_IS_PERIOD      (S_FREQ_MODE_MEASURE == FreqMeter::ModeMeasure::Period)

        enum E
        {
            Frequency,
            Period,
            Count
        };
    };

    // ����� ����� �������.
    struct TimeCounting
    {
#define S_FREQ_TIME_COUNTING            (set.freq.timeCounting)
#define S_FREQ_TIME_COUNTING_IS_100ms   (S_FREQ_TIME_COUNTING == FreqMeter::TimeCounting::_100ms)

        enum E
        {
            _100ms,
            _1s,
            _10s,
            Count
        };
    };
};
