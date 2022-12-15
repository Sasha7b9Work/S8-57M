// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <cstring>
#include <cmath>


// Установленное в true значение означает, что скриншот нужно заслать в устройство
//static int sendingString = -1;


void Painter::LoadPalette()
{
    HAL_LTDC::SetColors(&COLOR(0), Color::Count.value);
}


void Painter::SendRow(int row)
{
    uint8 *points = Display::GetBuffer() + row * Display::WIDTH;

    uint8 data[322] = { Command::Screen, static_cast<uint8>(row) };

    std::memcpy(&data[2], points, 320);

    HAL_BUS::SendToDevice(data, 322);
}
