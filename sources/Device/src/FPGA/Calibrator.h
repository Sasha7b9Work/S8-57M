// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


namespace Calibrator
{
    // Запускает процедуру калибровки
    void Calibrate();

    // Балансировать канал на всех диапазонах
    bool BalanceChannel(Ch::E, bool showHint);

    struct Mode
    {
#define S_SERV_CALIBRATOR_MODE  (set.serv.calibratorMode)

        enum E
        {
            AC,
            DC,
            GND
        };
    };
};
