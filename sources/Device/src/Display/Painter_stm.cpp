#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"


void Painter::Init()
{
}


void Painter::BeginScene(Color color)
{
    color.SetAsCurrent();

    // todo_paint
}


void Painter::EndScene()
{
    // todo_paint
}


void Painter::DrawTesterData(uint8 , Color , const uint16 *, const uint8 *)
{
    // todo_paint
}
