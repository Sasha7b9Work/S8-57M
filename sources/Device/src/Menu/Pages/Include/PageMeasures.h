// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A                 (S_CHANNEL_ENABLED_A && (S_MEAS_SOURCE_IS_A || S_MEAS_SOURCE_IS_A_B))
// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B                 (S_CHANNEL_ENABLED_B && (S_MEAS_SOURCE_IS_B || S_MEAS_SOURCE_IS_A_B))


// ������� �������������� ��������� ���������� �� �����
struct MeasuresOnDisplay
{
#define S_MEAS_ON_DISPLAY           (set.meas._number)
#define S_MEAS_ON_DISPLAY_IS_6_1    (S_MEAS_ON_DISPLAY == MeasuresOnDisplay::_6_1)
#define S_MEAS_ON_DISPLAY_IS_6_2    (S_MEAS_ON_DISPLAY == MeasuresOnDisplay::_6_2)

    enum E
    {
        _1,      // 1 ��������� ����� �����.
        _2,      // 2 ��������� ����� �����.
        _1_5,    // 1 ������ � 5 �����������.
        _2_5,    // 2 ������ �� 5 ���������.
        _3_5,    // 3 ������ �� 5 ���������.
        _6_1,    // 6 ����� �� 1 ���������.
        _6_2     // 6 ����� �� 2 ���������.
    };

    // ���������� true, ���� ��������� ����������� ����������� � ����� ������� ������
    static bool IsVertical();
};


struct MeasuresSource
{
#define S_MEAS_SOURCE           (set.meas._source)
#define S_MEAS_SOURCE_IS_A      (S_MEAS_SOURCE == MeasuresSource::A)
#define S_MEAS_SOURCE_IS_B      (S_MEAS_SOURCE == MeasuresSource::B)
#define S_MEAS_SOURCE_IS_A_B    (S_MEAS_SOURCE == MeasuresSource::A_B)

    enum E
    {
        A,
        B,
        A_B
    };
};


// ������� �� ������� ��� ������ ���������.
struct MeasuresModeViewSignals
{
#define S_MEAS_MODE_VIEW_SIGNALS                (set.meas._modeViewSignals)
#define S_MEAS_MODE_VIEW_SIGNALS_IS_COMPRESS    (S_MEAS_MODE_VIEW_SIGNALS == MeasuresModeViewSignals::Compress)

    enum E
    {
        AsIs,       // ���������� ������� ��� ����.
        Compress    // ������� ����� � ���������.
    };

    static bool IsCompress();
};


#define S_MEAS_SHOW     (set.meas._show)


struct SettingsAutoMeasures
{
    bool                        _show;              // ���������� �� ���������.
    MeasuresOnDisplay::E        _number;            // ������� ��������� ��������.
    MeasuresSource::E           _source;            // ��� ����� ������� �������� ���������.
    MeasuresModeViewSignals::E  _modeViewSignals;   // ������� �� ������� ��� ������ ���������.
    TypeMeasure::E              _measures[15];      // ��������� ��� ��������� ���������.
    TypeMeasure::E              _marked;            // ���������, �� ������� ����� �������� �������.
};



// ����� �������� ��������.
struct CursorsLookMode
{
#define S_CURS_LOOK_MODE(chan)              (set.curs._lookMode[chan])
#define S_CURS_LOOK_MODE_IS_VOLTAGE(chan)   (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Voltage)
#define S_CURS_LOOK_MODE_IS_TIME(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Time)
#define S_CURS_LOOK_MODE_IS_BOTH(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Both)

    enum E
    {
        None,      // ������� �� ������.
        Voltage,   // ������� ������ �� ����������� �������������.
        Time,      // ������� ������ �� �������� �������������.
        Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
    };
};

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursorsActive
{
#define S_CURS_ACTIVE       (set.curs._active)
#define S_CURS_ACTIVE_IS_T  (S_CURS_ACTIVE == CursorsActive::T)
#define S_CURS_ACTIVE_IS_U  (S_CURS_ACTIVE == CursorsActive::U)

    enum E
    {
        U,
        T,
        None
    };
};


// ����� �������� ���������
struct CursorsControl
{
#define S_CURS_CONTROL_U(chan)      (set.curs._cntrlU[chan])
#define S_CURS_CONTROL_T(chan)      (set.curs._cntrlT[chan])

    enum E
    {
        _1,        // ������
        _2,        // ������
        Both,      // ������
        Disabled    // �������
    };

    static bool IsDisabledT();
    static bool IsEnabled1T();
    static bool IsEnabled2T();

    static bool IsDisabledU();
    static bool IsEnabled1U();
    static bool IsEnabled2U();
};


// ������������ ����������� ��������.
struct CursorsMovement
{
#define S_CURS_MOVEMENT             (set.curs._movement)
#define S_CURS_MOVEMENT_IS_PIXELS   (S_CURS_MOVEMENT == CursorsMovement::Pixels)
#define S_CURS_MOVEMENT_IS_PERCENTS (S_CURS_MOVEMENT == CursorsMovement::Percents)

    enum E
    {
        Pixels,    // �� �������� ������
        Percents   // �� ���������
    };
};


#define S_CURS_SHOW                 (set.curs._showCursors)
#define S_CURS_SHOW_FREQ            (set.curs._showFreq)

#define S_CURS_SOURCE               (set.curs._source)
#define S_CURS_SOURCE_IS_A          (S_CURS_SOURCE == ChanA)

#define S_CURS_DU_PERCENTS(chan)    (set.curs._deltaU100percents[chan])
#define S_CURS_DT_PERCENTS(chan)    (set.curs._deltaT100percents[chan])

#define S_CURS_POS_U(chan, num)     (set.curs._posCurU[chan][num])
#define S_CURS_POS_U0(chan)         (S_CURS_POS_U(chan, 0))
#define S_CURS_POS_U1(chan)         (S_CURS_POS_U(chan, 1))

#define S_CURS_POS_T(chan, num)     (set.curs._posCurT[chan][num])
#define S_CURS_POS_T0(chan)         (S_CURS_POS_T(chan, 0))
#define S_CURS_POS_T1(chan)         (S_CURS_POS_T(chan, 1))


struct SettingsCursorsMeasures
{
    bool               _showCursors;             // ���������� �� �������.
    CursorsLookMode::E _lookMode[Ch::Count];   // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool               _showFreq;                // ������������� � true ��������, ��� ����� ���������� �� ������ 1/dT ����� ���������.
    CursorsActive::E   _active;                  // ����� ������� ����� �������.
    Ch::E              _source;                  // �������� - � ������ ������ ��������� �������.
    CursorsControl::E  _cntrlU[Ch::Count];     // �������� ������� ����������.
    CursorsControl::E  _cntrlT[Ch::Count];     // �������� ������� �������.
    CursorsMovement::E _movement;                // ��� ������������ �������� - �� ������ ��� ���������.
    float              _deltaU100percents[2];    // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float              _deltaT100percents[2];    // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    float              _posCurU[Ch::Count][2]; // ������� ������� �������� ���������� ����� �������.
    float              _posCurT[Ch::Count][2]; // ������� ������� �������� ������� ����� �������.
};


struct FuncModeDraw
{
#define S_MATH_MODE_DRAW                (set.math._modeDraw)
#define S_MATH_MODE_DRAW_IS_SEPARATE    (S_MATH_MODE_DRAW == FuncModeDraw::Separate)
#define S_MATH_MODE_DRAW_IS_DISABLED    (S_MATH_MODE_DRAW == FuncModeDraw::Disabled)

    enum E
    {
        Disabled,
        Separate,
        Together
    };
};


struct MathFunction
{
#define S_MATH_FUNCTION         (set.math._function)
#define S_MATH_FUNCTION_IS_SUM  (S_MATH_FUNCTION == MathFunction::Sum)
#define S_MATH_FUNCTION_IS_MUL  (S_MATH_FUNCTION == MathFunction::Mul)

    enum E
    {
        Sum,
        Mul
    };
};


struct ModeRegSet
{
#define S_MATH_MODE_REG_SET             (set.math._modeRegSet)
#define S_MATH_MODE_REG_SET_IS_RSHIFT   (S_MATH_MODE_REG_SET == ModeRegSet::RShift)
#define S_MATH_MODE_REG_SET_IS_RANGE    (S_MATH_MODE_REG_SET == ModeRegSet::Range)

    enum E
    {
        Range,
        RShift
    };
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E _modeDraw;       // ���������� ��� ����� ������� � ������ �������������� �������.
    MathFunction::E _function;
    ModeRegSet::E   _modeRegSet;     // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    Divider::E      _divider;
    int16           _rShift;
    Range::E        _range;
};



namespace PageMeasures
{
    extern const Page * const self;
};



namespace PageAutoMeasures
{
    extern const Page * const self;

    namespace Tune
    {
        extern const Page * const self;
    };
};



namespace PageCursorsMeasures
{
    extern const Page * const self;

    namespace Set
    {
        extern const Page * const self;

        void Draw_Channel(int x, int y);

        void OnPress_Channel();

        // ������� ��������� ������
        void IncCursCntrlU(Ch::E);

        // ������� ��������� ������
        void IncCursCntrlT(Ch::E);

        void OnPress_U();

        void OnPress_T();

        void SetCursPos100(Ch::E);

        // �������� �������� ������� ������� ���������� �� delta �����
        void SetShiftCursPosU(Ch::E, int numCur, float delta);

        // �������� �������� ������� ������� ������� �� delta �����
        void SetShiftCursPosT(Ch::E, int numCur, float delta);

        bool HandlerKey(const KeyEvent &event);

        void UpdateCursorsForLook();

        // ���������� ������� ������� ����������
        void SetCursorU(Ch::E, int numCur, float pos);

        // ���������� ������� ������� �������
        void SetCursorT(Ch::E, int numCur, float pos);

        // ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
        bool IsRegSetActiveOnCursors();
    };
};



namespace PageMath
{
    extern const Page * const self;
};
