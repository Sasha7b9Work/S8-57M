// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct AveragerTester
{
    // ������ ���������� ���������. ���� �������� ���������� �� ����, ������� ��� ������� ��� ���������� ������, ���������� ���������.
    static void SetCount(int enumAverage);

    static void Process(const uint16 *x, const uint8 *y, int step);

    static uint16 *X();
    static uint8 *Y();
};
