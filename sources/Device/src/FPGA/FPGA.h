// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


namespace FPGA
{
    bool IsRunning();

    // Включить/выключить калибратор.
    void LoadCalibratorMode();

    void GiveStart(uint16 pred, uint16 post);

    const int MAX_NUM_POINTS = 8 * 1024;

    // Принудительный запуск синхронизации
    void ForcedStart();

    void LoadRegUPR();

    void Reset();

    // True, если дан принудительный запуск
    extern bool forcedStart;

    extern uint16 post;
    extern uint16 pred;

    extern bool isRunning;

    namespace Flag
    {
        void Read(bool updateFreqMeter = true);

        void Clear();
        bool DataReady();
        bool TrigReady();
        bool Pred();
        bool HoldOff();

        bool FreqReady();
        bool PeriodReady();
        bool FreqOverflow();
        bool PeriodOverflow();
        bool FreqInProcess();
        bool PeriodInProcess();
    };

    void Init();
};
