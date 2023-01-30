// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


#define S_RSHIFT(channel)       set.ch[channel]._rShift
#define S_RSHIFT_A              S_RSHIFT(0)
#define S_RSHIFT_B              S_RSHIFT(1)
#define S_RSHIFT_MATH           set.math._rShift

#define S_CHANNEL_ENABLED(chan) set.ch[chan]._enabled
#define S_CHANNEL_ENABLED_A     S_CHANNEL_ENABLED(ChA)
#define S_CHANNEL_ENABLED_B     S_CHANNEL_ENABLED(ChB)

#define S_INVERSE(chan)         set.ch[chan]._inverse
#define S_INVERSE_A             S_INVERSE(ChA)
#define S_INVERSE_B             S_INVERSE(ChB)


struct SettingsChannel
{ //-V802
    int16         _rShift;    // Сдвиг канала по вертикали
    Range::E      _range;     // Масштаб канала по вертикали
    ModeCouple::E _couple;    // Связь по входу
    bool          _enabled;   // Включен/выключен канал
    Bandwidth::E  _bandwidth; // Ограничение полосы
    bool          _inverse;
    Divider::E    _divider;   // Множитель
};



namespace PageChannelA
{
    void OnChanged_Couple(bool active);

    extern const Page * const self;
};



namespace PageChannelB
{
    void OnChanged_Couple(bool active);

    extern const Page * const self;
};


namespace PageChannel
{
    extern void Enable(Ch::E, int enable);
};
