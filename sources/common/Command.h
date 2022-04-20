// 2022/04/20 16:51:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Command
{           //  ��������               0  |     1       |     2      |      3      |     4        |      5       |   6    |         ������
    enum E                                              
    {                                                   
/* 00 */    None,                   //    |             |            |             |              |              |        |
/* 01 */    ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |
/* 15 */    Display_Brightness,     // 15 | �������
/* 16 */    Screen,                 // 16 |  numRow     |
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
