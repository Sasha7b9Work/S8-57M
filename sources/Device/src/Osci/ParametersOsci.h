// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


#define ChanA Chan::A
#define ChanB Chan::B
#define setA  set.ch[ChanA]
#define setB  set.ch[ChanB]


struct DataSettings;
struct SettingsChannel;


#define S_TIME_BASE (set.time._base)
namespace TBase
{
    enum E
    {
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5,
        _10s,
        Count
    };

    pString Name(TBase::E tBase);

    pString ToString(TBase::E tBase);

    void Set(E v = Count);

    void Change(int delta);

    // � ����� �������� ������ ���������� ����� ����������� ������
    const E MIN_P2P = _10ms;

    // ����������� ������� �� �������, ��� ������� ��� �������� ��������� ������ �������� ���������
    const E MIN_PEAK_DET = _5us;

    // ���������� ����� ������� � ������ �������������
    int DeltaPoint();

    // ����� ����� �������
    float TimePoint(TBase::E base);
};


// ��������.
#define S_DIVIDER(channel)  (set.ch[channel]._divider)
#define S_DIVIDER_A         S_DIVIDER(0)
#define S_DIVIDER_B         S_DIVIDER(1)
#define S_DIVIDER_MATH      (set.math._divider)
namespace Divider
{
    enum E
    {
        _1,
        _10
    };

    inline int ToAbs(Divider::E v)
    {
        return (v == _1) ? 1 : 10;
    };
};


#define S_RANGE(channel)    set.ch[channel]._range
#define S_RANGE_A           S_RANGE(0)
#define S_RANGE_B           S_RANGE(1)
#define S_RANGE_MATH        set.math._range
namespace Range
{
    enum E
    {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Count
    };

    void Set(Chan::E ch, E range);

    pString Name(Chan::E ch);

    pString ToString(Chan::E ch, Divider::E divider);

    pString ToString(Range::E range, Divider::E divider);

    // ���������� ����������, ��������������� ������� ������� �����
    float MaxVoltageOnScreen(Range::E range);

    void Change(Chan::E ch, int16 delta);

    // ����������� ��� ��������� ����� ������ ��� ����� ������� ������������
    void LoadBoth();
};


namespace RShift
{
    // ���� ������������� ����� ���������� ��������
    void Set(Chan::E ch, int16 rShift);

    // �������� �� delta
    void Change(Chan::E ch, int16 delta);

    // ��������� � ����������
    void Load(Chan::E ch);

    // ���������� ��� �� ������
    void DrawBoth();

    // ������������� � ������
    String ToString(int16 rShiftRel, Range::E range, Divider::E divider);

    float ToAbs(int16 rShift, Range::E range);

    int16 ToRel(float rShiftAbs, Range::E range);

    bool ChangeMath(int delta);
};

// ����� ������ �� �����.
#define S_MODE_COUPLE(chan)         (set.ch[chan]._couple)
#define S_MODE_COUPLE_A             S_MODE_COUPLE(ChanA)
#define S_MODE_COUPLE_B             S_MODE_COUPLE(ChanB)
#define S_MODE_COUPLE_IS_DC(chan)   (S_MODE_COUPLE(chan) == ModeCouple::DC)
namespace ModeCouple
{
    enum E
    {
        DC,      // �������� ����
        AC,      // �������� ����
        GND,     // ���� �������.
        Count
    };

    void Set(Chan::E ch, ModeCouple::E couple);

    pString UGO(ModeCouple::E v);
};


#define S_BANDWIDTH(chan)           set.ch[chan]._bandwidth
#define S_BANDWIDTH_IS_20MHz(chan)  (S_BANDWIDTH(chan) == Bandwidth::_20MHz)
namespace Bandwidth
{
    enum E
    {
        Full,     // ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
        _20MHz,
    };

    void Load(Chan::E ch);
};


namespace TShift
{
    void Set(int);

    int Min();
    int Max();

    void Load();

    void Change(const int delta);

    String ToString(const TBase::E tBase = TBase::Count);

    float ToAbs(const int shift, const TBase::E tBase);

    void Reset();

    void Draw();
};

namespace Trig
{
    // ���������� true � ������ ������� ��������������
    bool SyncPulse();
};

namespace TrigLevel
{
    void Set(Chan::E ch, int16 level);

    // ��������� ������� ������������� � ���������� �����
    void Load();

    // �������� ������� ������������� �������� �������� ������������� �� delta ������
    void Change(int16 delta);

    // ���������� ����� ������� �������������
    void Find();

    void Draw();
};


namespace VALUE
{
    const uint8 AVE = 127;
    const uint8 MIN = AVE - 125;
    const uint8 MAX = AVE + 125;

    // ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
    const uint8 NONE = 0;

    void PointsToVoltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage);

    void PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    uint8 FromVoltage(float voltage, Range::E range, int16 rShift);

    float ToVoltage(uint8 value, Range::E range, int16 rShift);
};

