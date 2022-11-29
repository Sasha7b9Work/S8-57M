#pragma once


class ExtRAM
{
public:
    
    // ������ ���� ������� ������
    static uint8 *Begin();
    
    // �� ��������� ������ ������� ������
    static uint8 *End();
    
    // ���������� �� ������� ������ �� ������ address ������ buffer, size ������ 4
    static void Write(const uint8 *buffer, int size, uint8 *address);
    
    // ������ �� ������� ������ address � ����� buffer ������, size ������ 4
    static void Read(uint8 *buffer, int size, const uint8 *address);

    static void Fill(uint8 *begin = 0, uint8 value = 0, int size = 0);

    // ���������� ������� ��������� ������ ��������� �����������.
    static float Test1();
    static float Test2();

    // ���������� ����� � ��������, ������� �������� ������/������ sizekB �������� �� ������� ������. ���������� -1, ���� ��������� ������
    static float TestTime(int sizekB);
    
    // ���� �������� ������/������ 1�� ������� RAM
    static float TestTime1kB(uint8 *address);
};
