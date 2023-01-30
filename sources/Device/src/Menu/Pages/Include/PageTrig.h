// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


// �������� �������������
struct TrigSource
{
    static void Set(Ch::E = Ch::Count);
};


// ����� ������ ������������
#define S_TRIG_MODE_FIND            (set.trig._modeFind)
#define S_TRIG_MODE_FIND_IS_AUTO    (S_TRIG_MODE_FIND == TrigModeFind::Auto)
struct TrigModeFind
{
    enum E
    {
        Hand,      // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
        Auto       // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
    };
};


#define S_TRIG_INPUT    (set.trig._input)
struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    };
    // ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
    static void Load();

    static void Set(E v);
};


#define S_TRIG_POLARITY     (set.trig._polarity)
#define S_TRIG_POLARITY_U8  ((uint8)(S_TRIG_POLARITY))
struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    };

    static void Load();

    static void Set(E v);
};


// ����� �������.
#define S_TRIG_START_MODE           (set.trig._startMode)
#define S_TRIG_START_MODE_IS_SINGLE (S_TRIG_START_MODE == TrigStartMode::Single)
#define S_TRIG_START_MODE_IS_AUTO   (S_TRIG_START_MODE == TrigStartMode::Auto)
struct TrigStartMode
{
    enum E
    {
        Auto,     // ��������������.
        Wait,     // ������.
        Single,   // �����������.
        Count
    };
    static void Set(TrigStartMode::E);
};


#define S_TRIG_SOURCE           (set.trig._source)
#define S_TRIG_SOURCE_IS_A      (S_TRIG_SOURCE == ChA)

#define S_TRIG_LEVEL(channel)   (set.trig._level[channel])
#define S_TRIG_LEVEL_SOURCE     S_TRIG_LEVEL(S_TRIG_SOURCE)

//#define S_TRIG_HOLDOFF_VALUE    (set.trig._holdOff)
//#define S_TRIG_HOLDOFF_ENABLED  (set.trig._holdOffEnabled)


struct SettingsTrig
{
    Ch::E            _source;
    TrigInput::E     _input;
    TrigPolarity::E  _polarity;
    int16            _level[Ch::Count];
    TrigStartMode::E _startMode;         // ����� �������.
    TrigModeFind::E  _notUsed2;          // ����� ������������� - ������� ��� �������������.
                                         // \todo ����������� trig_holdOff ������������
    int16            _hotUsed0;          // ��������� �������������
    bool             _notUsed1;          // ��������� ��������� �������������
};


namespace PageTrig
{
    extern const Page * const self;

    namespace HoldOff
    {
        extern const Page * const self;
    };

    namespace Find
    {
        extern const Page * const self;
    };
};
