// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"


void Painter::Init()
{
}


void Painter::BeginScene(int field, Color color)
{
    color.SetAsCurrent();

    HAL_BUS::Panel::Send(Command::Paint_BeginScene, (uint8)field);
}


void Painter::EndScene()
{
    HAL_BUS::Panel::Send(Command::Paint_EndScene);
}
