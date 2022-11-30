// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CursorsMeasurements
{
    // ���������� ������� � ���������� ��������� ���������
    static void Draw();
    
    // ���������� true,���� ����� �������� �������
    static bool  NecessaryDraw();
    
    // �������� ������� ������� ����������
    static float PosU(Chan::E ch, int numCur);
    
    // ���������� �������� ������� �������
    static float PosT(Chan::E ch, int num);
    
    // �������� ������ ������� ����������
    static String Voltage(Chan::E source, int numCur);
    
    // ������� ������ ������� �������
    static String Time(Chan::E source, int numCur);
    
    // ���������� ������ ���������� ��������� �� ����������
    static String PercentsU(Chan::E source);
    
    // ���������� ������ ���������� ��������� �� �������
    static String PercentsT(Chan::E source);

    static void SetCursPosT_temp(Chan::E ch, int num, float value);
};
