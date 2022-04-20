// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"


void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}
