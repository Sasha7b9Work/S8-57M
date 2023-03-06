// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


#define S_DBG_SHOW_CONSOLE              (set.dbg.showConsole)
#define S_DBG_NUM_STRINGS_IN_CONSOLE    (set.dbg.numStrings)
#define S_DBG_SHOW_ALL_REGISTERS        (set.dbg.showAll)
#define S_DBG_SHOW_FLAG                 (set.dbg.showFlag)
#define S_DBG_SHOW_RSHIFT(chan)         (set.dbg.showRShift[chan])
#define S_DBG_SHOW_TRIG_LEV             (set.dbg.showTrigLev)
#define S_DBG_SHOW_RANGE(chan)          (set.dbg.showRange[chan])
#define S_DBG_SHOW_CHAN_PARAM(chan)     (set.dbg.showChanParam[chan])
#define S_DBG_SHOW_TRIG_PARAM           (set.dbg.showTrigParam)
#define S_DBG_SHOW_TSHIFT               (set.dbg.showTShift)
#define S_DBG_SHOW_TBASE                (set.dbg.showTBase)
#define S_DBG_SHOW_STATS                (set.dbg.showStats)
#define S_DBG_RUN_TEST                  (set.dbg.runTest)
#define S_DBG_SHOW_BATTERY              (set.dbg.showBattery)
#define S_DBG_SHOW_RAND_GATES           (set.dbg.showRandGate)
#define S_DBG_SHOW_RAND_PRED_POST       (set.dbg.showRandPredPost)


struct SettingsDebug
{ //-V802
    int8   showConsole;         // Показывать ли консоль
    int16  numStrings;          // Число строк в консоли.
    bool   showAll;
    bool   showFlag;
    bool   showRShift[2];
    bool   showTrigLev;
    bool   showRange[2];
    bool   showChanParam[2];
    bool   showTrigParam;
    bool   showTShift;
    bool   showTBase;
    bool   showStats;           // Показывать статистику на экране (fps, например).
    bool   runTest;             // Если true, то нужно выполнять тест при запуске
    bool   showBattery;         // Показывать или нет состояние батареи
    bool   showRandGate;        // Выводить ли значения ворот в режиме рандомизатора
    bool   showRandPredPost;    // Выводить ли значения пред- и после- запуска в режиме рандомизатора
};


namespace PageDebug
{
    extern const Page *const self;

    namespace PageConsole
    {
        extern const Page *const self;
    };


    namespace PageRegisters
    {
        extern const Page *const self;
    };


    namespace PageRand
    {
        extern const Page *const self;
    };


    namespace PageSettings
    {
        extern const Page *const self;
    };


    namespace PageSerialNumber
    {
        extern const Page *const self;
    };


    namespace PageTests
    {
        extern const Page *const self;

        void OnPress_Run();
    };


    namespace PageADC
    {
        extern const Page *const self;
    };
};
