// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font_p.h"


namespace Painter
{
    // ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    void EndScene();

    // ���������� ������ � ������
    void SendRow(int row);

    void SetColor(Color color);

    // ���������� ������� ���� ���������
    Color GetColor();

    // ������������� ����� �������� ��������. ����������� � ������� LoadPalette() ��� SetPalette()
    void SetColorValue(Color color, uint value);

    // ��������� � ������� ��� �����
    void LoadPalette();

    // ���������� ���� ���������
    // ���������� ����� ������� ������
    void SetPoint(int x, int y);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);

    // ���������� �������������� �����
    void DrawHLine(int y, int x0, int x1);

    // ���������� ������������ �����
    void DrawVLine(int x, int y0, int y1);

    // ���������� ������������ �����
    void DrawLine(int x0, int y0, int x1, int y1);

    // \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    // dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // ������ ����������� ������������ �����.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height);

    void FillRegion(int x, int y, int width, int height);

    uint ReduceBrightness(uint colorValue, float newBrightness);

    int DrawFormatText(int x, int y, char *format, ...);

    // ����� ������ � ������� x, y
    int DrawFormText(int x, int y, Color color, pString text, ...);

    // �������� ������ ����������� numString � ����������
    void SendScreenToDevice();
};
