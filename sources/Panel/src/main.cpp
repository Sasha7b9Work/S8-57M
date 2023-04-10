// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"


int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();

    while (1)
    {
        DEBUG_POINT;

        static uint prevTime = 0;

        DEBUG_POINT;

        HAL_BUS::Update();

        DEBUG_POINT;

        if (TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }

        DEBUG_POINT;
    }
}
