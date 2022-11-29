// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Grid
{
    // ��������� ������������ ��� ������� ��������, ������� �������� ����������� ��� ����� �������� � ���������� ����� - ��������, ��������� ����
    const int WIDTH = 280;
    const int HEIGHT = 200;
    const int SIZE_CELL = 20;

    void Draw();

    // ��� �������� ����� �������� � ����������� �� ������������� ������
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int ChannelCenterHeight();
    int ChannelHeight();
    int FullBottom();
    int MathBottom();
    int MathHeight();
    int ChannelBottom();
    // ���������� ������ y-���������� ����, � ������� ����� �������� ��������������
    int BottomForWarnings();
};
