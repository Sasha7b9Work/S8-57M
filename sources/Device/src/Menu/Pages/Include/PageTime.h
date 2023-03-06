// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


#define S_PEAK_DET          (set.time.peakDet)
#define S_PEAK_DET_ENABLED  (S_PEAK_DET == PeakDetMode::Enabled)
struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    };

    static bool IsEnabled();

    static void Set(E v);
};


// ��������� ����� ������������� �� �������.
#define S_TPOS              (set.time.tPos)
#define S_TPOS_IS_LEFT      (S_TPOS == TPos::Left)
#define S_TPOS_IS_CENTER    (S_TPOS == TPos::Center)
struct TPos
{
    enum E
    {
        Left,          // �������� � ������ ����.
        Center,        // �������� � ������.
        Right          // �������� � ������� ����.
    };

    // ������ �������� ���������� ������ ������������� � ������
    static int InBytes();

    static void Draw();

    static int PosX();

    static void Set(E v);
};


// ��� ������� ��� ������ �������������.
#define S_RAND_SAMPLE_TYPE              (set.time.sampleType)
struct SampleType
{
    enum E
    {
        Equal,  // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Real    // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    };

    // ������� ��������� �������� ������, ��� ��� ����������� ������� � ������������� ����� ���� ������ Real
    static bool IsReal();
};


// ������� ��/���.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    };
};


// ��� �������� � �������� �� �����������
#define S_LINKING_TSHIFT    (set.time._1)
struct LinkingTShift
{
    enum E
    {
        Time,     // �������� ��������� � �������
        Position  // �������� ��������� � �������
    };
};


#define S_TIME_SHIFT    (set.time.shift)

 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
    SampleType::E       sampleType;
    FunctionTime::E     _0;
    LinkingTShift::E    _1;  // ��� �������� �������� �� �����������
};



namespace PageTime
{
    void OnChanged_PeakDet(bool active);

    void OnChanged_TPos(bool active);

    void SetSampleTime(SampleType::E v);

    extern const Page * const self;
};
