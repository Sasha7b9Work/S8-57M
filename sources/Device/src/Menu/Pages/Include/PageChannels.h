#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


#define S_RSHIFT(channel)       set.ch[channel]._rShift
#define S_RSHIFT_A              S_RSHIFT(0)
#define S_RSHIFT_B              S_RSHIFT(1)
#define S_RSHIFT_MATH           set.math._rShift

#define S_CHANNEL_ENABLED(chan) set.ch[chan]._enabled
#define S_CHANNEL_ENABLED_A     S_CHANNEL_ENABLED(ChanA)
#define S_CHANNEL_ENABLED_B     S_CHANNEL_ENABLED(ChanB)

#define S_INVERSE(chan)         set.ch[chan]._inverse
#define S_INVERSE_A             S_INVERSE(ChanA)
#define S_INVERSE_B             S_INVERSE(ChanB)


struct SettingsChannel
{ //-V802
    int16         _rShift;    // ����� ������ �� ���������
    Range::E      _range;     // ������� ������ �� ���������
    ModeCouple::E _couple;    // ����� �� �����
    bool          _enabled;   // �������/�������� �����
    Bandwidth::E  _bandwidth; // ����������� ������
    bool          _inverse;
    Divider::E    _divider;   // ���������
};



struct PageChannelA
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};



struct PageChannelB
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};


struct PageChannel
{
    static void Enable(Chan::E ch, int enable);
};
