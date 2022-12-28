// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"


namespace Painter
{
    static int field = 0;

    int CurrentField()
    {
        return field;
    }
}


void Painter::BeginScene(int _field, Color color)
{
    field = _field;

    color.SetAsCurrent();

    SBuffer(Command::Paint_BeginScene, (uint8)_field).Send();
}


void Painter::EndScene()
{
    SBuffer(Command::Paint_EndScene).Send();
}
