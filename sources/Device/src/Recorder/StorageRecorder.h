// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/DeviceSettings.h"


struct Record;


struct Point16
{
    uint8 max;
    uint8 min;

    Point16() : max(0), min(255) {}

    Point16(BitSet16 p) : max(p.byte0), min(p.byte1) { }

    bool IsEmpty() const { return (min == 255) && (max == 0); };

    Point16 *Next(Record *record) const;

    void Erase() { min = 255; max = 0; }

    BitSet16 ToBitSet16() { return BitSet16(min, max); }
};


// ����� �������� �����, ������� ���� ��������� �� ����� ������ �� �������
struct BufferMissingPoints
{
    // ���������� ���������� ���������� �����
    static int Size() { return last - first; };

    // ����� ��� ����� � �����
    static void Push(BitSet16 a, BitSet16 b);

    // ��������� ����� �� ������
    static void Pop(BitSet16 *a, BitSet16 *b);

private:
    // ����� ����������� ����������� �����
    static BitSet16 points[2][256];

    // ������ "�����������" �����
    static int last;

    // ������ ������ �����
    static int first;
};


struct PointFloat
{
    float min;
    float max;

    // �������������� ����� � ������
    void Prepare();

    bool IsEmpty() const;

    void Add(float value);
};


// ��������� ������ ������������ - ������� ������ �����
struct Record
{
    PackedTime timeStart;       // ����� ������ ������ �����
    int        numPoints;       // ����� ���������� �����
    uint       timeForPointMS;  // ���������� ����� �������
    uint8      sources;         // ����� �������� ������.
                                // ��� 0 - ����� 1; ��� 1 - ����� 2; ��� 2 - ������. ������ � ����� ������� ����������� ����� ��������������� ���������� � ��������� ����� ��������� Record
    uint8      bytesOnPoint;    // ������� ���� ����� �� ���� ����� ���� ����������

    uint8      offsetB;         // �������� �������� ������ B
    uint8      offsetSensor;    // �������� �������� �������
    uint16     maxPoints;       // ���� �� ����� 0, �� ����� ������ ���������� maxPoints �������. ��� ����������� ����� ��������� ������ �����.

    // ���������������� ��������� ����� ������� ������
    void Init();

    // ���������� �������� �����
    void AddPoints(BitSet16 dataA, BitSet16 dataB);

    // �������� �����, ������� ���� ��������� �� ����� ���������
    void AddMissingPoints();

    // ���������� ����� ������� � ������
    void AddPoint(float value);

    // ����� ����� � ��������������
    int NumPoints() const;

    // ���������� ������ ��������� ������, ��������� ��� �������� ����� ������
    uint FreeMemory() const;

    // ���������� true, ���� ������ ��������
    bool IsValid() const;

    // ���������� ����� ������� ����� ������ (������ � �������) �����
    uint8 *End() const;

    // ����� ������ ������
    uint8 *Begin() const;

    // ��������� �� ����� number ������ A
    Point16 *ValueA(int number);

    // ��������� �� ����� number ������ B
    Point16 *ValueB(int number);

    // ��������� �� ����� number �������
    PointFloat *ValueSensor(int number);

    // ���������� true, ���� ������ �������� ������ ������ A
    bool ContainsChannelA() const;

    // ���������� true, ���� ������ �������� ������ ������ B
    bool ContainsChannelB() const;

    // ���������� true, ���� ������ �������� ������ �������
    bool ContainsSensor() const;

    // ������������� ������������ ���������� ����� ��� ������
    void SetMaxPoints(uint16 max) {  maxPoints = max; }

private:
    
    // � ����� ������ ���������� ������
    uint8 *BeginData();

    // ��������� �� ������ ������ ����� � ������� number
    uint8 *AddressPoints(int number);

    // ��������������� ����� ����� ��������� num1 � num2
    void Interpolate(int num1, int num2);

    // ������� �����, ���� ����������
    void DeleteOldPoints();
};


struct StorageRecorder
{
    // ������������� ����� �������� � ����� "�����������"
    static void Init();

    // ������ ����� ������ ��� �������� ������ � ���������
    static bool CreateNewRecord();

    // ������ ������ ��� "�������������".
    static bool CreateListeningRecord();

    // ���������� ��������� �� ������� ������
    static Record *LastRecord();

    // ���������� ���������� ���������� �������. 0 - ���������, 1 - �������������. ���� ��� ������, �� � 0-�� ������ ����������� ����� ������
    static uint NumRecords();
};
