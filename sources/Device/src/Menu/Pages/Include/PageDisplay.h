// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


struct DisplayMapping
{
#define S_DISP_MAPPING          (set.disp._mapping)
#define S_DISP_MAPPING_IS_LINES (S_DISP_MAPPING == DisplayMapping::Lines)
#define S_DISP_MAPPING_IS_DOTS  (S_DISP_MAPPING == DisplayMapping::Dots)

    enum E
    {
        Lines,
        Dots,
        Count
    };
};


// ���������� ���������� �� ����������.
struct ENumAverage
{
#define S_OSCI_ENUM_AVERAGE         (set.disp._enumAverage)
#define S_OSCI_AVERAGING_IS_ENABLED (S_OSCI_ENUM_AVERAGE != ENumAverage::_1)
#define S_OSCI_ENUM_AVERAGE_U16     ((uint16)(S_OSCI_ENUM_AVERAGE))
#define S_OSCI_NUM_AVERAGE          (1 << S_OSCI_ENUM_AVERAGE)

#define S_TEST_ENUM_AVERAGE         (set.test._enumAverage)

    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        Count
    };
    static void Set(ENumAverage::E v);
};


// ���������� ����������.
struct ENumAccum
{
#define S_DISP_ENUM_ACCUM               (set.disp._enumAccum)
#define S_DISP_NUM_ACCUM                (1 << S_DISP_ENUM_ACCUM)
#define S_DISP_ENUM_ACCUM_IS_INFINITY   (set.disp._enumAccum == ENumAccum::_Infinity)

    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _Infinity,
        Count
    };

    static pString ToString(ENumAccum::E v);
};


// ���������� ����� ��� ������� �����������.
struct ENumSmoothing
{
#define S_DISP_ENUM_SMOOTH  (set.disp._enumSmoothing)
#define S_DISP_NUM_SMOOTH   (S_DISP_ENUM_SMOOTH + 1)

    enum E
    {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };
};


// ����������� FPS.
struct ENumSignalsInSec
{
#define S_DISP_ENUM_FPS (set.disp._enumSignalsInSec)

    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };
    // ���������� ���������� ������������ ����� �������
    static uint TimeBetweenFramesMS();
};


// ��� ����� �� ������.
struct TypeGrid
{
#define S_DISP_TYPE_GRID        (set.disp._typeGrid)
#define S_DISP_TYPE_GRID_IS_1   (S_DISP_TYPE_GRID == TypeGrid::_1)
#define S_DISP_TYPE_GRID_IS_2   (S_DISP_TYPE_GRID == TypeGrid::_2)
#define S_DISP_TYPE_GRID_IS_3   (S_DISP_TYPE_GRID == TypeGrid::_3)

    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    };
};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
#define S_MENU_AUTOHIDE (set.disp._menuAutoHide)

    enum E
    {
        None,   // �������.
        _5,     // ����� 5 ������.
        _10,    // ����� 10 ������.
        _15,    // ����� 15 ������.
        _30,    // ����� 30 ������.
        _60     // ����� 60 ������.
    };
};


#define S_DISP_BRIGHTNESS                   (set.disp._brightness)
#define S_DISP_LAST_AFFECTED_CHANNEL        (set.disp._lastAffectedChannel)
#define S_DISP_LAST_AFFECTED_CHANNEL_IS_A   (S_DISP_LAST_AFFECTED_CHANNEL == Chan::A)

struct SettingsDisplay
{ //-V802
    DisplayMapping::E      _mapping;
    ENumAverage::E         _enumAverage;            // ����� ���������� �������.
    ENumAccum::E           _enumAccum;              // ����� ���������� ������� �� ������.
    ENumSmoothing::E       _enumSmoothing;          // ������������ ���������� ����� ��� ����������� �������.
    ENumSignalsInSec::E    _enumSignalsInSec;       // ������������ ����� ���������� ������� � �������.
    TypeGrid::E            _typeGrid;               // ��� �����
    int16                  _brightness;             // ������� �������.
    MenuAutoHide::E        _menuAutoHide;           // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                  _shiftInMemory;          // \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                    // ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                _lastAffectedChannel;    // \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                    // �� ����� � ���������� ���� ������ �������
    void SetLastAffectedChannel(Chan::E ch)
    {
        _lastAffectedChannel = ch;
    }
};


namespace PageDisplay
{
    extern const Page * const self;

    void SetTypeGrid(TypeGrid::E);

    void SetBrightness(int value);

    namespace Accumulation
    {
        extern const Page * const self;

        void OnPress_Accumulation_Clear();
    };


    namespace Settings
    {
        extern const Page * const self;

        namespace Colors
        {
            void Init();

            extern const Page * const self;

            // ��� �����
            extern ColorType colorTypeGrid;

            // ��� ������� ������
            extern ColorType colorTypeA;

            // ��� ������� ������
            extern ColorType colorTypeB;
        };
    };
};
