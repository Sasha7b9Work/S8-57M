// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/AveragerTester.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Display/MemPainter.h"
#include "Painter_common.h"
#include <cstring>
#include <cmath>


void Painter::LoadPalette()
{
    HAL_LTDC::SetColors(&COLOR(0), Color::NUMBER.value);
}


void Painter::DrawTesterData(uint8 mode, Color color, const uint16 _x[TESTER_NUM_POINTS], const uint8 _y[TESTER_NUM_POINTS]) //-V2009
{
    color.SetAsCurrent();

    int step = EXTRACT_STEP(mode);

    int numAverage = EXTRACT_ENUM_AVERAGE(mode);
   
    AveragerTester::SetCount(numAverage);
    AveragerTester::Process(_x, _y, step);

    uint16 *x = AveragerTester::X();
    uint8 *y = AveragerTester::Y();
    
    if(EXTRACT_MODE_DRAW(mode))
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            MemPainter::FillRegion(x[i], y[i], 2, 2);
        }
    }
    else
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            MemPainter::DrawLine(x[i], y[i], x[i + 1], y[i + 1]);
        }
    }
}


uint Painter::ReduceBrightness(uint colorValue, float newBrightness)
{
    int red = static_cast<int>(static_cast<float>(R_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(red, 0, 0xff); //-V2516
    int green = static_cast<int>(static_cast<float>(G_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(green, 0, 0xff); //-V2516
    int blue = static_cast<int>(static_cast<float>(B_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(blue, 0, 0xff); //-V2516
    return MAKE_COLOR(red, green, blue);
}
