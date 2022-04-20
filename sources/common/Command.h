// 2022/04/20 16:51:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Command
{           //  ��������               0  |     1       |     2      |      3      |     4        |      5       |   6    |         ������
    enum E                                              
    {                                                   
/* 00 */    None,                   //    |             |            |             |              |              |        |
/* 01 */    ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |
/* 02 */    Display_Brightness,     // 15 | �������
/* 03 */    Screen,                 // 16 |  numRow     |
/* 04 */    AddToConsole,           // 21 | num symb    |  ..... symbols ....

/* 05 */    Paint_DirectLine,       // ��������� "������" �����
            //  ��� ������� [8] | ����� ���� � ������� (������� �� ����������) [8] | ����� ����� [8] | ���������� ����� [8]

/* 06 */    Paint_DifferentLine,    // ��������� "����������" �����
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
