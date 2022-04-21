#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include "Display/Display.h"
#include <cstring>


namespace Painter
{
    static const int SIZE_BUFFER = Display::WIDTH * Display::HEIGHT / 2;

    uint8 buffer[SIZE_BUFFER];
    uint8 front[10 * 1024];
}


void Painter::Init()
{
}


void Painter::BeginScene(Color color)
{
    color.SetAsCurrent();

    std::memset(buffer, Color::GetCurent().value, SIZE_BUFFER);
}


void Painter::EndScene()
{
    // todo_paint
}


void Painter::DrawTesterData(uint8 , Color , const uint16 *, const uint8 *)
{
    // todo_paint
}
