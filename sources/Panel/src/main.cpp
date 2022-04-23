// 2022/04/20 16:50:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "Display/MemPainter.h"
#include "common/Communicator/Communicator_.h"
#include <cstdlib>


int main()
{
    HAL::Init();

    Keyboard::Init();

    while (1)
    {
        static uint prevTime = 0;

        HAL_BUS::Update();

        if (TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }

        Communicator::Panel::Update();

        MemPainter::BeginPaint(0, 0, 640, 480);

        MemPainter::Fill();

        MemPainter::EndPaint();
    }
}
