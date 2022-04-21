#pragma once


namespace ExtRAM
{
    // ������ ���� ������� ������
    uint8* Begin();

    // �� ��������� ������ ������� ������
    uint8* End();

    // ���������� �� ������� ������ �� ������ address ������ buffer, size ������ 4
    void Write(const uint8* buffer, int size, uint8* address);

    // ������ �� ������� ������ address � ����� buffer ������, size ������ 4
    void Read(uint8* buffer, int size, const uint8* address);

    void Fill(uint8* begin = 0, uint8 value = 0, int size = 0);

    // ���������� ������� ��������� ������ ��������� �����������.
    float Test1();
    float Test2();

    // ���������� ����� � ��������, ������� �������� ������/������ sizekB �������� �� ������� ������. ���������� -1, ���� ��������� ������
    float TestTime(int sizekB);

    // ���� �������� ������/������ 1�� ������� RAM
    float TestTime1kB(uint8* address);
};
