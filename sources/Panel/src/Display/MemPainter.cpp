// 2022/04/22 11:40:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/MemPainter.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Font/Font_p.h"
#include "Display/Painter_common.h"
#include <cstring>


#define CALCULATE_X()  x -= x0; if (x < 0) { return; } if (x >= width)  { return; }
#define CALCULATE_Y()  y -= y0; if (y < 0) { return; } if (y >= height) { return; }


namespace MemPainter
{
    uint8 buffer[64 * 1024] __attribute__((section("CCM_DATA")));;

    int x0 = 0;
    int y0 = 0;
    int width = 0;
    int height = 0;
}


void MemPainter::BeginPaint(int x, int y, int w, int h)
{
    x0 = Math::Limitation(x, 0, Display::WIDTH - 1);
    y0 = Math::Limitation(y, 0, Display::HEIGHT - 1);

    width = Math::Limitation(w, 0, 640);
    height = Math::Limitation(h, 0, 480);

    if (width + x0 >= Display::WIDTH)
    {
        width = Display::WIDTH - x0;
    }

    if (height + y0 >= Display::HEIGHT)
    {
        height = Display::HEIGHT - y0;
    }
}


void MemPainter::SetPoint(int x, int y)
{
    CALCULATE_X();
    CALCULATE_Y();

    *(buffer + y * Display::WIDTH * y + x) = currentColor.value;
}


void MemPainter::Fill()
{
    std::memset(buffer, currentColor.value, (uint)(width * height));
}


void MemPainter::EndPaint()
{
    HAL_LTDC::CopyImage(buffer, x0, y0, width, height);
}
