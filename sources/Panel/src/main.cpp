// 2022/04/20 16:50:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "Display/MemPainter.h"
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

        MemPainter::BeginPaint(0, 0, 640, 480);

        MemPainter::Fill();

        MemPainter::EndPaint();
    }
}
