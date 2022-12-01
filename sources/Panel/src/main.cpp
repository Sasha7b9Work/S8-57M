// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "UnusedMemory.h"


int main()
{
    CPU::Init();

    Display::Init();

    UnusedMemory::Init();

    Keyboard::Init();

    while(1)
    {
        HAL_BUS::Update();

        Keyboard::Update();
    }
}
