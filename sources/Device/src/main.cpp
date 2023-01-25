// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Timer.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Test/Test.h"



int main(void)
{
    Device::Init();

    Timer::PauseOnTime(1);

//    AT25160N::Init();

//    AT25160N::Test();
    
//    Test::Run();
  
    uint time_start = 0;        // Âנולÿ חאלונא ןונגמדמ פנוילא
    int num_frames = 0;

    while (1)
    {
        num_frames++;

        Device::Update();

        if (TIME_MS >= time_start + 1000)
        {
//            LOG_WRITE("fps %d", num_frames);
            num_frames = 0;
            time_start = TIME_MS;
        }
    }
}
