#include "defines.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/Beeper.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


static void OnPress_Screen()
{
    if (S_FFT_ENABLED)
    {
        Display::ShowWarning("��������� ���������� ���");
    }
    else
    {
        Math::CircleIncrease<int8>(reinterpret_cast<int8 *>(&S_MATH_MODE_DRAW), 0, 2);
    }
}

static void Draw_Screen_Disable(int x, int y)
{
    String("���").Draw(x + 2, y + 5);
}

static void Draw_Screen_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine line(13);
    line.Draw(x + 3, y + 9);
    line.Draw(x + 3, y + 10);
}

static void Draw_Screen_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}

static void Draw_Screen(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    const pFuncDraw funcs[3] =
    {
        Draw_Screen_Disable,
        Draw_Screen_Separate,
        Draw_Screen_Together
    };
    funcs[S_MATH_MODE_DRAW](x, y);
}

DEF_GRAPH_BUTTON_HINTS_3( bScreen,                                                                                                                         //--- ������ - ������� - ����� ---
    "�����",
    "�������� ����� ����������� ��������������� �������",
    &PageMath::self, Item::Active, OnPress_Screen, Draw_Screen,
    Draw_Screen_Disable, "����� �������������� ������� ��������",
    Draw_Screen_Separate, "������� � �������������� ������� ��������� � ������ �����",
    Draw_Screen_Together, "������� � �������������� ������� ��������� � ����� ����"
)


static void OnPress_Type()
{
    Math::CircleIncrease<int8>(reinterpret_cast<int8 *>(&S_MATH_FUNCTION), 0, 1);
}

static void Draw_Type_Sum(int x, int y)
{
    HLine(10).Draw(x + 4, y + 9);
    VLine(10).Draw(x + 9, y + 4);
}

static void Draw_Type_Mul(int x, int y)
{
    Char(SymbolUGO2::MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
}

static void Draw_Type(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    const pFuncDraw funcs[2] = { Draw_Type_Sum, Draw_Type_Mul };
    funcs[S_MATH_FUNCTION](x, y);
}

static bool IsActive_Type()
{
    return !S_MATH_MODE_DRAW_IS_DISABLED;
}

DEF_GRAPH_BUTTON_HINTS_2( bType,                                                                                                                             //--- ������ - ������� - ��� ---
    "���",
    "����� �������������� �������",
    &PageMath::self, IsActive_Type, OnPress_Type, Draw_Type,
    Draw_Type_Sum, "��������",
    Draw_Type_Mul, "���������"
)


static void OnPress_ModeArrows()
{
    Math::CircleIncrease<int8>(reinterpret_cast<int8*>(&S_MATH_MODE_REG_SET), 0, 1);
}

static void Draw_ModeArrows_Range(int x, int y)
{
    Char('M').Draw(x + 7, y + 5);
}

static void Draw_ModeArrows_RShift(int x, int y)
{
    String("��").Draw(x + 5, y + 5);
}

static void Draw_ModeArrows(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw funcs[2] = { Draw_ModeArrows_Range, Draw_ModeArrows_RShift };
    funcs[S_MATH_MODE_REG_SET](x, y);
}

static bool IsActive_ModeArrows()
{
    return !S_MATH_MODE_DRAW_IS_DISABLED;
}

DEF_GRAPH_BUTTON_HINTS_2( bModeArrows,                                                                                                     //--- ������ - ������� - ����� ����� ��������� ---
    "����� ����� ���������",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    &PageMath::self, IsActive_ModeArrows, OnPress_ModeArrows, Draw_ModeArrows,
    Draw_ModeArrows_Range, "���������� ���������",
    Draw_ModeArrows_RShift, "���������� ���������"
)


static void OnPress_RangeA()
{
    S_RANGE_MATH = S_RANGE_A;
    S_DIVIDER_MATH = S_DIVIDER_A;
}

static void Draw_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

static bool IsActive_RangeA()
{
    return !S_MATH_MODE_DRAW_IS_DISABLED;
}

DEF_GRAPH_BUTTON( bRangeA,                                                                                                                   //--- ������ - ������� - ������� 1-�� ������ ---
    "������� 1-�� ������",
    "���������� ������� ������� ������ ��� ����������� ����������",
    &PageMath::self, IsActive_RangeA, OnPress_RangeA, Draw_RangeA
)


static void OnPress_RangeB()
{
    S_RANGE_MATH = S_RANGE_B;
    S_DIVIDER_MATH = S_DIVIDER_B;
}

static void Draw_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

static bool IsActive_RangeB()
{
    return !S_MATH_MODE_DRAW_IS_DISABLED;
}

DEF_GRAPH_BUTTON( bRangeB,                                                                                                                   //--- ������ - ������� - ������� 2-�� ������ ---
    "������� 2-�� ������",
    "���������� ������� ������� ������ ��� ����������� ����������",
    &PageMath::self, IsActive_RangeB, OnPress_RangeB, Draw_RangeB
)


static bool IsActive_Math()
{
    return !S_FFT_ENABLED;
}

static void OnOpenClose_Math(bool)
{
    if (S_FFT_ENABLED)
    {
        Display::ShowWarning("��������� ���������� ���");
    }
}

static bool HandlerKey_Function(const KeyEvent &event) // -V2506
{
    if (S_MATH_MODE_DRAW_IS_DISABLED)
    {
        return false;
    }

    int delta = 0;

    if(event.IsArrowUp() || event.IsArrowRight())
    {
        delta = 1;
    }
    else if(event.IsArrowDown() || event.IsArrowLeft()) //-V2516
    {
        delta = -1;
    }

    if(delta != 0)
    {
        if (S_MATH_MODE_REG_SET_IS_RSHIFT)
        {
            if (RShift::ChangeMath(delta))
            {
                Beeper::RegulatorShiftRotate();
            }
        }
        else if (S_MATH_MODE_REG_SET_IS_RANGE) //-V2516
        {
            static int sum = 0;
            sum -= delta;

            float rShiftAbs = RShift::ToAbs(S_RSHIFT_MATH, S_RANGE_MATH);

            if(sum > 2)
            {
                if(S_RANGE_MATH < Range::Count - 1)
                {
                    S_RANGE_MATH = static_cast<Range::E>(static_cast<uint8>(S_RANGE_MATH + 1));  // SET_RANGE_MATH++;
                    S_RSHIFT_MATH = RShift::ToRel(rShiftAbs, S_RANGE_MATH);
                    Beeper::RegulatorSwitchRotate();
                }
                sum = 0;
            }
            else if(sum < -2) //-V2516
            {
                if(S_RANGE_MATH > 0)
                {
                    S_RANGE_MATH = static_cast<Range::E>(static_cast<uint8>(S_RANGE_MATH - 1));  // SET_RANGE_MATH--;
                    S_RSHIFT_MATH = RShift::ToRel(rShiftAbs, S_RANGE_MATH);
                    Beeper::RegulatorSwitchRotate();
                }
                sum = 0;
            }
        }

        return true;
    }

    return false;
}

DEF_PAGE_5( pMath,                                                                                                                                         //--- ������ - ������� --- //-V2567
    "����������",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    &bScreen,
    &bType,
    &bModeArrows,
    &bRangeA,
    &bRangeB,
    PageName::Math, &PageMeasures::self, IsActive_Math, Page::NormalTitle, OnOpenClose_Math, Page::BeforeDraw, HandlerKey_Function
)

const Page * const PageMath::self = static_cast<const Page *>(&pMath);
