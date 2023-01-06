#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"


void Painter::BeginScene(Color color)
{
    color.SetAsCurrent();

    SBuffer(Command::Paint_BeginScene).Send();
}


void Painter::EndScene()
{
    SBuffer(Command::Paint_EndScene).Send();
}
