// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


#define S_DBG_SHOW_CONSOLE              (set.dbg._showConsole)
#define S_DBG_NUM_STRINGS_IN_CONSOLE    (set.dbg._numStrings)
#define S_DBG_SHOW_ALL_REGISTERS        (set.dbg._showAll)
#define S_DBG_SHOW_FLAG                 (set.dbg._showFlag)
#define S_DBG_SHOW_RSHIFT(chan)         (set.dbg._showRShift[chan])
#define S_DBG_SHOW_TRIG_LEV             (set.dbg._showTrigLev)
#define S_DBG_SHOW_RANGE(chan)          (set.dbg._showRange[chan])
#define S_DBG_SHOW_CHAN_PARAM(chan)     (set.dbg._showChanParam[chan])
#define S_DBG_SHOW_TRIG_PARAM           (set.dbg._showTrigParam)
#define S_DBG_SHOW_TSHIFT               (set.dbg._showTShift)
#define S_DBG_SHOW_TBASE                (set.dbg._showTBase)
#define S_DBG_SHOW_STATS                (set.dbg._showStats)
#define S_DBG_RUN_TEST                  (set.dbg._runTest)
#define S_DBG_SHOW_BATTERY              (set.dbg._showBattery)
#define S_DBG_SHOW_RAND_GATES           (set.dbg._showRandGate)
#define S_DBG_SHOW_RAND_PRED_POST       (set.dbg._showRandPredPost)


struct SettingsDebug
{ //-V802
    int8   _showConsole;         // ���������� �� �������
    int16  _numStrings;          // ����� ����� � �������.
    bool   _showAll;
    bool   _showFlag;
    bool   _showRShift[2];
    bool   _showTrigLev;
    bool   _showRange[2];
    bool   _showChanParam[2];
    bool   _showTrigParam;
    bool   _showTShift;
    bool   _showTBase;
    bool   _showStats;           // ���������� ���������� �� ������ (fps, ��������).
    bool   _runTest;             // ���� true, �� ����� ��������� ���� ��� �������
    bool   _showBattery;         // ���������� ��� ��� ��������� �������
    bool   _showRandGate;        // �������� �� �������� ����� � ������ �������������
    bool   _showRandPredPost;    // �������� �� �������� ����- � �����- ������� � ������ �������������
};


struct PageDebug
{
    static const Page *const self;

    struct PageConsole
    {
        static const Page *const self;
    };


    struct PageRegisters
    {
        static const Page *const self;
    };


    struct PageRand
    {
        static const Page *const self;
    };


    struct PageSettings
    {
        static const Page *const self;
    };


    struct PageSerialNumber
    {
        static const Page *const self;
    };


    struct PageTests
    {
        static const Page *const self;

        static void OnPress_Run();
    };


    struct PageADC
    {
        static const Page *const self;
    };
};
