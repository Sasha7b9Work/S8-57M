// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"


namespace Grid
{
    // Нарисовать сетку для режима осциллографа
    static void DrawOsci();
    // Нарисовать сетку для режима тестер-компонента
    static void DrawTester();

    static void DrawRecorder();

    static void DrawGridSignal(int left, int top, int width, int height);

    static void DrawGridSpectrum();
    // Возвращает расстояние между 
    static int  DeltaHforLineGrid();

    static int  DeltaVforLineGrid();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);

    static float DeltaY();

    static float DeltaX();

    static int MathTop();
}


int Grid::Left()
{
    static const int left[Device::Mode::Count] = { SIZE_CELL, 0, 0, 0 };

    return left[(int)(Device::CurrentMode())];
}


int Grid::Top()
{
    static const int top[Device::Mode::Count] = { SIZE_CELL - 1, 0, 0, 0 };

    return top[(int)(Device::CurrentMode())];
}


int Grid::Width()
{
    static const int width[Device::Mode::Count] = { WIDTH, Display::WIDTH - 1, 0, Display::WIDTH - 1 };

    return width[(int)(Device::CurrentMode())];
}


int Grid::Height()
{
    static const int height[Device::Mode::Count] = { HEIGHT, Display::HEIGHT - 1, 0, Display::HEIGHT - 1 };

    return height[(int)(Device::CurrentMode())];
}


int Grid::Bottom()
{
    return Top() + Height();
}


int Grid::FullBottom()
{
    return Bottom();
}


int Grid::Right()
{
    return Left() + Width();
}


int Grid::MathTop()
{
    return Grid::Top() + Grid::Height() / 2;
}


int Grid::MathBottom()
{
    return FullBottom();
}


int Grid::MathHeight()
{
    return Height() / 2;
}


int Grid::ChannelBottom()
{
    return (Display::IsSeparate()) ? (Top() + Height() / 2) : FullBottom();
}


int Grid::ChannelCenterHeight()
{
    return (Top() + Bottom()) / 2;
}


int Grid::ChannelHeight()
{
    return (ChannelBottom() - Top());
}


void Grid::Draw()
{
    static const pFuncVV funcs[4] =
    {
        DrawOsci,
        DrawTester,
        EmptyFuncVV,
        DrawRecorder
    };

    funcs[(int)(Device::CurrentMode())]();
}


void Grid::DrawOsci()
{
    if (Display::IsSeparate())
    {
        DrawGridSignal(Grid::Left(), Grid::Top(), Grid::Width(), Grid::Height() / 2);

        if (S_FFT_ENABLED)
        {
            DrawGridSpectrum();
        }

        HLine(Grid::Width()).Draw(Grid::Left(), Grid::Top() + Grid::Height() / 2, Color::FILL);
    }
    else
    {
        DrawGridSignal(Grid::Left(), Grid::Top(), Grid::Width(), Grid::Height());
    }
}


void Grid::DrawGridSignal(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Color::FILL.SetAsCurrent();

    if (top == Grid::Top())
    {
        HLine(left - 4).Draw(1, top);
        HLine(left - 4).Draw(1, bottom);
        HLine(Display::WIDTH - right - 4).Draw(right + 2, top);
        HLine(Display::WIDTH - right - 4).Draw(right + 2, bottom);

        VLine line(bottom - top - 4);

        line.Draw(0, top + 2);
        line.Draw(Display::WIDTH - 1, top + 2);
    }

    float deltaX = DeltaX() * (float)width / width;
    float deltaY = DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    float centerX = (float)(left + width / 2);
    float centerY = (float)(top + height / 2);

    Color::GRID.SetAsCurrent();

    if (S_DISP_TYPE_GRID_IS_1)
    {
        DrawGridType1(left, top, right, bottom, centerX, deltaX, deltaY, stepX, stepY);
    }
    else if (S_DISP_TYPE_GRID_IS_2)
    {
        DrawGridType2(left, top, right, bottom, (int)(deltaX), (int)(deltaY), (int)(stepX), (int)(stepY));
    }
    else if (S_DISP_TYPE_GRID_IS_3)
    {
        DrawGridType3(left, top, right, bottom, (int)(centerX), (int)(centerY), (int)(deltaX), (int)(deltaY), (int)(stepX));
    }
}


void Grid::DrawGridSpectrum()
{
    if (S_FFT_SCALE_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static pcharc strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[S_FFT_MAX_DB];
        float scale = (float)Grid::MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = MathTop() + (int)(i * scale);

			HLine(256).Draw(Grid::Left(), y, Color::GRID);

            Color::FILL.SetAsCurrent();
            String((char *)(strs[i])).Draw(3, y - 4);
        }

        Color::FILL.SetAsCurrent();
        String("дБ").Draw(5, MathTop() + 1);
    }
    else // SCALE_FFT_IS_LINEAR
    {
        static pcharc strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = MathTop() + (int)(i * scale);

			HLine(256).Draw(Grid::Left(), y, Color::GRID);

            String((char *)(strs[i])).Draw(5, y - 4, Color::FILL);
        }
    }

    VLine(Grid::MathBottom() - MathTop()).Draw(Grid::Left() + 256, MathTop(), Color::FILL);
}


float Grid::DeltaY()
{
    float delta = (Grid::FullBottom() - Grid::Top()) / 10.0F;
    return Display::IsSeparate() ? (delta / 2.0F) : delta;
}


float Grid::DeltaX()
{
    float delta = (Grid::Right() - Grid::Left()) / 14.0F;
    return delta;
}


void Grid::DrawGridType1(int left, int top, int right, int bottom, float centerX, float dX, float dY, float stepX, float stepY)
{
    {
        uint16 masX[17];
        masX[0] = (uint16)(left + 1);
        for (int i = 1; i < 7; i++)
        {
            masX[i] = (uint16)(left + (int)(dX * i));
        }
        for (int i = 7; i < 10; i++)
        {
            masX[i] = (uint16)((int)(centerX)-8 + i);
        }
        for (int i = 10; i < 16; i++)
        {
            masX[i] = (uint16)(centerX + dX * (i - 9)); //-V2004
        }
        masX[16] = (uint16)(right - 1);

        MultiVPointLine(17, masX, (int)stepY, DeltaVforLineGrid()).Draw(top + (int)stepY, Color::GRID);
    }

    {
        uint16 mas[13];

        mas[0] = (uint16)(top + 1);

        for (int i = 1; i < 10; i++)
        {
            mas[i] = (uint16)(top + (int)(dY * i));
        }

        mas[10] = (uint16)(top + (int)(dY * 5) - 1);
        mas[11] = (uint16)(top + (int)(dY * 5) + 1);

        mas[12] = (uint16)(bottom - 1);

        MultiHPointLine(13, mas, (int)stepX, DeltaHforLineGrid()).Draw(left + (int)stepX, Color::GRID);
    }
}


void Grid::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    masX[14] = (uint16)(right - 1);
    MultiVPointLine(15, masX, stepY, DeltaVforLineGrid()).Draw(top + stepY, Color::GRID);

    uint16 mas[11];
    mas[0] = (uint16)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint16)(top + (int)(deltaY * i));
    }
    mas[10] = (uint16)(bottom - 1);

    MultiHPointLine(11, mas, stepX, DeltaHforLineGrid()).Draw(left + stepX, Color::GRID);
}


void Grid::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX)
{
    HPointLine(right - left - stepX, (float)stepX).Draw(left + stepX, centerY);

    uint16 masY[6] = {
        (uint16)(top + 1),
        (uint16)(top + 2),
        (uint16)(centerY - 1),
        (uint16)(centerY + 1),
        (uint16)(bottom - 2),
        (uint16)(bottom - 1)
    };

    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, Color::GRID);

    VPointLine(bottom - top - 2 * stepX, (float)stepX).Draw(centerX, top + stepX, Color::GRID);

    uint16 masX[6] =
    {
        (uint16)(left + 1),
        (uint16)(left + 2),
        (uint16)(centerX - 1),
        (uint16)(centerX + 1),
        (uint16)(right - 2),
        (uint16)(right - 1)
    };

    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, Color::GRID);
}


int Grid::DeltaVforLineGrid()
{
    int result = 49;

    if (S_MEAS_SHOW && MeasuresModeViewSignals::IsCompress())
    {
        switch (S_MEAS_ON_DISPLAY)
        {
        case MeasuresOnDisplay::_1_5: result = S_MEAS_SOURCE_IS_A_B ? 55 : 59; break;
        case MeasuresOnDisplay::_2_5: result = S_MEAS_SOURCE_IS_A_B ? 69 : 51; break;
        case MeasuresOnDisplay::_3_5: result = S_MEAS_SOURCE_IS_A_B ? 54 : 68; break;
        case MeasuresOnDisplay::_1:
        case MeasuresOnDisplay::_2:
        case MeasuresOnDisplay::_6_1:
        case MeasuresOnDisplay::_6_2:
            break;
        }
    }

    return result;
}


int Grid::DeltaHforLineGrid()
{
    int result = 69;

    if (MeasuresModeViewSignals::IsCompress())
    {
        if (S_MEAS_ON_DISPLAY_IS_6_1)
        {
            result = 73;
        }
        else if (S_MEAS_ON_DISPLAY_IS_6_2)
        {
            result = 83;
        }
    }

    return result;
}


void Grid::DrawTester()
{
    Color::FILL.SetAsCurrent();

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0);

    float x0 = 0;

    float y0 = 0;

    Color::GRID.SetAsCurrent();

    int x = (int)(x0 + Display::WIDTH / 2);
    int y = (int)(y0 + Display::HEIGHT / 2);

    VLine(Display::HEIGHT).Draw(x, 0);

	HLine(Display::WIDTH).Draw(0, y);

    Color::GRID.SetAsCurrent();

    int deltaX = 32;
    int deltaY = 24;

    x += deltaX;

    float deltaPoint = 5.0F;

    VPointLine vLine(Display::HEIGHT, deltaPoint);

    while (x < Display::WIDTH)
    {
        vLine.Draw(x, 0);
        x += deltaX;
    }

    x = (int)(x0 + Display::WIDTH / 2 - deltaX);

    while (x > 0)
    {
        vLine.Draw(x, 0);
        x -= deltaX;
    }

    y += deltaY;

    HPointLine hLine(Display::WIDTH, deltaPoint);

    while (y < Display::HEIGHT)
    {
        hLine.Draw(0, y);
        y += deltaY;
    }

    y = (int)(y0 + Display::HEIGHT / 2 - deltaY);

    while (y > 0)
    {
        hLine.Draw(0, y);
        y -= deltaY;
    }
}


void Grid::DrawRecorder()
{
    Color::GRID.SetAsCurrent();

    VLine vLine(Display::HEIGHT - 1);

    int step = 20;

    for (int x = 0; x < Display::WIDTH; x += step)
    {
        vLine.Draw(x, 0);
    }

    HLine hLine(Display::WIDTH - 1);

    for (int y = 0; y < Display::HEIGHT; y += 24)
    {
        hLine.Draw(0, y);
    }

    HLine(Display::WIDTH - 1).Draw(0, Display::HEIGHT / 2);

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
}


int Grid::BottomForWarnings()
{
    int result = Bottom();

    Page *page = Menu::OpenedPage();

    if (Menu::IsShown() && page)
    {
        result = Display::HEIGHT - page->Height() - 1;
    }

    return result;
}
