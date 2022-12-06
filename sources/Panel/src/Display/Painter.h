// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/Font/Font_p.h"


struct Painter
{
    // ���������� ������ � ����������
    static void SendRow(int row);

    // ��������� � ������� ��� �����
    static void LoadPalette();

    static void DrawVPointLine(int x, int y, int delta, int count);

    static void DrawHPointLine(int x, int y, int delta, int count);

    // ���������� �������������� �����
    static void DrawHLine(int y, int x0, int x1);

    // ���������� ������������ �����
    static void DrawVLine(int x, int y0, int y1);

    // ���������� ������������ �����
    static void DrawLine(int x0, int y0, int x1, int y1);

    // \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    // dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // ������ ����������� ������������ �����.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static int DrawFormatText(int x, int y, char *format, ...);

    // ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);

    // �������� ������ ����������� numString � ����������
    static void SendScreenToDevice();

private:
    static void FillRegion(int x, int y, int width, int height);

    // ���������� ����� ������� ������
    static void SetPoint(int x, int y);
};
