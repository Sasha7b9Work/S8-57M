// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CursorsMeasurements
{
    // ���������� ������� � ���������� ��������� ���������
    void Draw();

    // ���������� true,���� ����� �������� �������
    bool  NecessaryDraw();

    // �������� ������� ������� ����������
    float PosU(Chan::E ch, int numCur);

    // ���������� �������� ������� �������
    float PosT(Chan::E ch, int num);

    // �������� ������ ������� ����������
    String Voltage(Chan::E source, int numCur);

    // ������� ������ ������� �������
    String Time(Chan::E source, int numCur);

    // ���������� ������ ���������� ��������� �� ����������
    String PercentsU(Chan::E source);

    // ���������� ������ ���������� ��������� �� �������
    String PercentsT(Chan::E source);

    void SetCursPosT_temp(Chan::E ch, int num, float value);
};
