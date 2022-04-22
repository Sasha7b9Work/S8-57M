// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font_p.h"


struct Painter
{
    // ���������� ������ ������-����������. � ������� ���� - ������� ��� ������� ��������. � ������� ������� - ���������� ����������
    static void DrawTesterData(uint8 mode, Color color, const uint16 x[TESTER_NUM_POINTS], const uint8 y[TESTER_NUM_POINTS]);

    // \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    // dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static int DrawFormatText(int x, int y, char* format, ...);

    // ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);
};
