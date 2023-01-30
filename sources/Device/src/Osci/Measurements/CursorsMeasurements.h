// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CursorsMeasurements
{
    // ���������� ������� � ���������� ��������� ���������
    void Draw();

    // ���������� true,���� ����� �������� �������
    bool  NecessaryDraw();

    // �������� ������� ������� ����������
    float PosU(Ch::E, int numCur);

    // ���������� �������� ������� �������
    float PosT(Ch::E, int num);

    // �������� ������ ������� ����������
    String Voltage(Ch::E source, int numCur);

    // ������� ������ ������� �������
    String Time(Ch::E source, int numCur);

    // ���������� ������ ���������� ��������� �� ����������
    String PercentsU(Ch::E source);

    // ���������� ������ ���������� ��������� �� �������
    String PercentsT(Ch::E source);

    void SetCursPosT_temp(Ch::E, int num, float value);
};
