// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/stm429.h"


class CPU : public STM429
{
public:

    static void Init();

private:

    static void SystemClockConfig();
};
