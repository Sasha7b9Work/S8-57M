// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct FPGA
{
    static bool IsRunning();

    // Включить/выключить калибратор.
    static void LoadCalibratorMode();

    static void GiveStart(uint16 pred, uint16 post);

    static const int MAX_NUM_POINTS = 8 * 1024;

    // Принудительный запуск синхронизации
    static void ForcedStart();

    static void LoadRegUPR();

    static void Reset();

    // True, если дан принудительный запуск
    static bool forcedStart;

    static uint16 post;
    static uint16 pred;

    static bool isRunning;

    struct Flag
    {
        static void Read(bool updateFreqMeter = true);

        static void Clear();
        static bool DataReady();
        static bool TrigReady();
        static bool Pred();
        static bool HoldOff();

        static bool FreqReady();
        static bool PeriodReady();
        static bool FreqOverflow();
        static bool PeriodOverflow();
        static bool FreqInProcess();
        static bool PeriodInProcess();
    };

    static void Init();

private:

    static void OnPressStart();
};
