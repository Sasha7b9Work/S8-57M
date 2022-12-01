// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


namespace TableMeasures
{
    // ���������� ���������� �������������� ���������
    void Draw();

    int NumCols();

    int NumRows();

    // �� ������� ������� ����� �� �����������
    int GetDeltaGridLeft();

    // ���������� ������ ���� ��� ������ ��������������� ���������
    int DY();

    // ���������� ������ ���� ��� ������ ��������������� ���������
    int DX();


    struct Cell
    {
    public:
        Cell(int _row, int _col) : row(_row), col(_col) {};
        void Draw(int x, int y);

    private:
        void DrawStringMeasure(int x, int y);

        int row;
        int col;
    };
};


// �������� ������ ���������
struct PageChoiceMeasures
{
    // ��������� ������� ��� �������� ��������
    static void OnKeyEvent(const KeyEvent &event);
    // ����� �������� ��� ��������/�������� �������� ������
    void OnOpenCloseEvent();
    // ��������/��������� �����������
    static void ChangeState();

    static void Draw();
};


struct AutoMeasurements
{
    // \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    // �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    static void SetData();

    // ���������� true, ���� ������ �����������
    static bool DataIsSetting();

    // ��������� ��� �������������� ���������
    static void CalculateMeasures();
    
    // ���������� �������� ���������
    static Measure GetActiveMeasure();

    static char *Freq2String(float, bool, char buffer[20]);

    static char *Float2String(float, bool, char buffer[20]);

    static int markerTime[Chan::Count][2];

    static int markerVoltage[Chan::Count][2];

    // ������� ��������� ��������� (�� ������� ������)
    static int8 posActive;

private:

    // �������� �������� ������ ch �� ��������� � ��������
    static void CountedToCurrentRShift(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // ������ �� IN_A, IN_B ����������� � ������� ���������� � �������� � OUT_A, OUT_B
    static void CountedToCurrentSettings();

    static void CountedToCurrentSettings(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // ������� �������� ������ � ����������� ��������
    static void LimitationData(uint8 *inOut, uint numBytes);
};


// �������� �� ���������� ���������� �������� � SCPI
struct AutoMeasuresSender
{
friend struct AutoMeasurements;

    // ��������� ������ ��� ��������� � ������� � SCPI
    static void DesignateForSending(TypeMeasure::E type);

private:

    // ��� ���������, ������� ����� ������ � SCPI. ���� Count, �� �� ���� ������ ��������
    static TypeMeasure::E sended;
};
