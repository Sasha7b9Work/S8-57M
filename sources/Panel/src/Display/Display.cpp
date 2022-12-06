// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Text/Text.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Display/BackBuffer.h"
#include <cstdlib>



namespace Display
{
    LTDC_HandleTypeDef hltdc;

    uint8  front[Display::WIDTH * Display::HEIGHT];

    uint8 *buffer = front;

    static void DrawStartScreen();
}


void Display::Init()
{
    HAL_DAC2::Init();
    HAL_LTDC::Init(reinterpret_cast<uint>(buffer));
    Painter::LoadPalette();

    DrawStartScreen();
}


uint8 *Display::GetBuffer()
{
    return buffer;
}


uint8 *Display::GetBufferEnd()
{
    return buffer + Display::WIDTH * Display::HEIGHT;
}


void Display::DrawStartScreen()
{
    for (int field = 0; field < 5; field++)
    {
        Color::BACK.SetAsCurrent();

        BackBuffer::BeginPaint(field);

        Color::FILL.SetAsCurrent();

        PFont::Set(PTypeFont::_GOST28);
        Text::SetSpacing(3);

        int x0 = 85;
        int dX = 65;
        int y0 = 50;

        int x1 = 120;
        int y1 = 130;

        Text::Draw(x0, y0, "ÎÀÎ");

        Text::Draw(x0 + dX, y0, "ÌÍÈÏÈ");

        Text::Draw(x1, y1, "Ñ8-57");

        BackBuffer::EndPaint();
    }
}


uint Display::Update()
{
    uint start = TIME_MS;

    for (int field = 0; field < 5; field++)
    {
        Color::BLACK.SetAsCurrent();

        BackBuffer::BeginPaint(field);

        Update1();

        BackBuffer::EndPaint();
    }

    return TIME_MS - start;
}


void Display::Update1()
{
    static const int width = 100;
    static int x = 0;
    static int y = 0;

    static int dX = 1;
    static int dY = 1;

    Color::WHITE.SetAsCurrent();

    BackBuffer::FillRect(x + 1, y + 1, width - 2, width - 2);

    Color::BLACK.SetAsCurrent();

//    Painter::DrawRectangle(x, y, width, width);

    x += dX;
    y += dY;

    if (x > Display::WIDTH - width)
    {
        dX = -1;
        x += dX;
    }

    if (x < 0)
    {
        dX = 1;
        x += dX;
    }

    if (y > Display::HEIGHT - width)
    {
        dY = -1;
        y += dY;
    }

    if (y < 0)
    {
        dY = 1;
        y += dY;
    }

    Color::WHITE.SetAsCurrent();

    Painter::SetPoint(0, 0);
    Painter::SetPoint(Display::WIDTH - 1, 0);
    Painter::SetPoint(Display::WIDTH - 1, Display::HEIGHT - 1);
    Painter::SetPoint(0, Display::HEIGHT - 1);
}
