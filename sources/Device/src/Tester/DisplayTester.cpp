#include "defines.h"
#include "Display/Console.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Osci/Reader.h"
#include "Settings/Settings.h"
#include "Tester/Tester.h"
#include "Utils/Values.h"


// � ����� ������� �������� ��������� ����� ������-����������
typedef uint8 array8[Tester::NUM_STEPS][TESTER_NUM_POINTS];
typedef uint16 array16[Tester::NUM_STEPS][TESTER_NUM_POINTS];

bool DisplayTester::ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array8 *datY = (array8 *)OUT_A;
static array16 *datX = (array16 *)OUT_B;

void DisplayTester::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    for (int i = 0; i < Tester::NUM_STEPS; i++)
    {
        DrawData(i);
    }

    DrawLegend(274, 2);

    DrawParametersChannel(ChanA, 3, 3);

    DrawParametersChannel(ChanB, 240, 206);

    Rectangle(::Display::WIDTH - 1, ::Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
    
    Menu::Draw();
}


Color DisplayTester::ColorForStep(int _step)
{
    static const Color colors[Tester::NUM_STEPS] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};

    if (_step < Tester::NUM_STEPS)
    {
        return colors[_step];
    }

    return Color::FILL;
}


void DisplayTester::DrawData(int numStep)
{
    if(!ready[numStep])
    {
        return;
    }

    uint16 *x = &(*datX)[numStep][0];
    uint8 *y = &(*datY)[numStep][0];
    
    uint8 mode = BUILD_MODE(S_TEST_VIEW_MODE, numStep, S_TEST_ENUM_AVERAGE);

    Painter::DrawTesterData(mode, ColorForStep(numStep), x, y);
}


void DisplayTester::SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS])
{
    ready[numStep] = true;

    uint16 *x = &(*datX)[numStep][0];
    uint8 *y = &(*datY)[numStep][0];

    for(int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        x[i] = dx[i];
        y[i] = dy[i];
    }
}


void DisplayTester::DrawLegend(int x, int y)
{
    Region(43, 40).Fill(x, y, Color::BACK);

    HLine line(9);

    for (int i = 0; i < Tester::NUM_STEPS; i++)
    {
        line.Draw(x + 1, y + 4 + i * 8, ColorForStep(i));

        ValueForStep(i).Draw(x + 12, y + i * 8);
    }
}


String DisplayTester::ValueForStep(int step)
{
    static pString valuesU[2][5] =
    {
        {"0 �", "0.6 �", "1.2 �", "1.8 �", "2.4 �"},
        {"0 �", "3 �",   "6 �",   "9 �",   "12 �"}
    };

    if (S_TEST_CONTROL_IS_VOLTAGE)
    {
        return String(valuesU[S_TEST_STEP_U][step]);
    }

    static pString valuesI[2][5] =
    {
        {"0 ���", "4 ���",  "8 ���",  "12 ���", "16 ���"},
        {"0 ���", "20 ���", "40 ���", "60 ���", "80 ���"}
    };

    return String(valuesI[S_TEST_STEP_I][step]);
}


void DisplayTester::DrawParametersChannel(Chan::E ch, int x, int y)
{
    int16 rShift = S_RSHIFT(ch);
    Tester::Scale scale(S_RANGE(ch), ch);
    Tester::Shift shift(rShift, ch);

    Color::FILL.SetAsCurrent();
    Text(scale.ToString()).DrawOnBackground(x, y, Color::BACK);
    Color::FILL.SetAsCurrent();

    Text(shift.ToString(scale.value).c_str()).DrawOnBackground(x + ((ch == ChanA) ? 25 : 35), y, Color::BACK);
}
