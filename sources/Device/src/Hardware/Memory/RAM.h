// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataSettings;
struct FrameP2P;
struct Packet;


class RAM
{
public:

    static bool canTrace;

    // ����� �������� � ����� �������. 0 - ��������� ��������� ������, 1 - ������������� ��������� ������.
    static int16 currentSignal;

    // �������������� �������������� ��������� ������
    static void Init();

    // ����������� � ��������� ����� ��� �������� ����� ������
    static DataSettings *PrepareForNewData();

    // ������� ���������� � num � ����� ���������� ������. 0 - ��������� ������, 1 - �������������
    static DataSettings *Get(int num = 0);

    // ���������� ���������� ������� � ������
    static int NumberDatas();

    // ���������� true, ���� ��������� ���������� ����������� ������� ������������� �������
    static bool LastSettingsEqualsCurrent();

    // ����� ������ ���� ������� ����� ����������� ����� ����� ��� �������������. ������������ ����� � �������� ������ ����� ������ � ������������� (� �������� ������ �����) ������
    // ������������� � ���� � ��� �� �����
    static void NewFrameForRandomize() { needNewFrame = true; }

private:
    // ��������� �� ����� ������ ���������� �����. �� ����� ���� ������
    static Packet *oldest;

    // ��������� �� ��������� ���������� �����. �� ����� ���� ���������
    static Packet *newest;

    // ���������� ����� ��� ������ ������ � ������� � ������������ � ds. ���������� �����
    static uint AllocateMemoryForPacket(const DataSettings *ds);

    // ������� ����� ������ ������
    static void RemoveOldest();

    // ���������� size ���� ������ � ������ ������
    static void AllocateMemoryFromBegin(int size);

    static bool needNewFrame;
};
