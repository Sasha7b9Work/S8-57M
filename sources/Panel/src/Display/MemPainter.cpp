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


namespace MemPainter
{
    uint8 buffer[64 * 1024] __attribute__((section("CCM_DATA")));;

    int width = 0;
    int height = 0;
}


void MemPainter::BeginPaint(int w, int h)
{
    width = Math::Limitation(w, w, 640);
    height = Math::Limitation(h, h, 480);
}


void MemPainter::SetPoint(int x, int y)
{
    *(buffer + y * Display::WIDTH * y + x) = currentColor.value;
}


void MemPainter::Fill()
{
    std::memset(buffer, currentColor.value, (uint)(width * height));
}


void MemPainter::EndPaint(int x, int y)
{
    HAL_LTDC::CopyImage(buffer, x, y, width, height);
}
