// 2022/04/20 16:51:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Command
{           //  ��������               0  |     1       |     2      |      3      |     4        |      5       |   6    |         ������
    enum E                                              
    {                                                   
/* 00 */    None,                   //    |             |            |             |              |              |        |
/* 01 */    ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |
/* 12 */    Paint_SetPoint,         // 12 |   x[0:7]    |  x[8:15]   |      y      |              |              |        |
/* 13 */    Paint_DrawLine,         // 13 |  x0[0:7]    | x0[8:15]   |     y0      |   x1[0:7]    |   x1[8:15]   |   y1   |
// � ������ ����� �������� ����� - ������� ��� == 0 - �����, 0 - �����
// ���� 1..3 - ����� ����
// � ������� ��������� �������� ���������� ���������� 0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64, 7 - 128, 8 - 256
/* 14 */    Paint_TesterLines,      // 14 |   mode      |   Color    | x[TESTER_NUM_POINTS]        y[TESTER_NUM_POINTS]         3 + 240 * 2 + 240 = 723
/* 15 */    Display_Brightness,     // 15 | �������
/* 16 */    Screen,                 // 16 |  numRow     |
/* 17 */    Paint_VPointLine,       // 17 |   x[0:7]    |  x[8:15]   |      y      |   delta      | count        |
/* 18 */    Paint_HPointLine,       // 18 |   x[0:7]    |  x[8:15]   |      y      |   delta      | count        |
/* 19 */    Paint_SetMinWidthFont,  // 19 |   width     |
/* 20 */    Paint_SetTextSpacing,   // 20 |   0 | 1     |
/* 21 */    AddToConsole,           // 21 | num symb    |  ..... symbols ....

            PNT_DirectLine,         // ��������� "������" �����
            //  ��� ������� [8] | ����� ���� � ������� (������� �� ����������) [8] | ����� ����� [8] | ���������� ����� [8]

            PNT_DifferentLine,      // ��������� "����������" �����
            //  ��� ������� [8] | ����� ����� � ������� (������� �� ����������) [8] | �������� ����� �� ��������� � ������� ������ [8] | ���������� ����� [8]

            Count
    };
};


#define TESTER_NUM_POINTS 120


#define SIZE_STRING (320)


// ��������� ����� ��������� - ������� (1) ��� ������� (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)
