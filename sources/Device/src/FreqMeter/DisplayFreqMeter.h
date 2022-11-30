// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


namespace DisplayFreqMeter
{
    // Отрисовка значения
    void Update();

    // Выводит отладочную информацию
    void DrawDebugInfo();
};


namespace ProgressBarFreqMeter
{
    void Draw(int x, int y);
};
