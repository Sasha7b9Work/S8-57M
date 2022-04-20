// 2022/04/20 16:50:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>



int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();

    while(1)
    {
        static uint prevTime = 0;

        HAL_BUS::Update();

        if(TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }
    }
}
