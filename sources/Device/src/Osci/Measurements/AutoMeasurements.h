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
namespace PageChoiceMeasures
{
    // ��������� ������� ��� �������� ��������
    void OnKeyEvent(const KeyEvent &event);

    // ��������/��������� �����������
    void ChangeState();

    void Draw();
};


namespace AutoMeasurements
{
    // \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    // �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    void SetData();

    // ���������� true, ���� ������ �����������
    bool DataIsSetting();

    // ��������� ��� �������������� ���������
    void CalculateMeasures();

    // ���������� �������� ���������
    Measure GetActiveMeasure();

    char *Freq2String(float, bool, char buffer[20]);

    char *Float2String(float, bool, char buffer[20]);

    extern int markerTime[Ch::Count][2];

    extern int markerVoltage[Ch::Count][2];

    // ������� ��������� ��������� (�� ������� ������)
    extern int8 posActive;
};


// �������� �� ���������� ���������� �������� � SCPI
namespace AutoMeasuresSender
{
    // ��������� ������ ��� ��������� � ������� � SCPI
    void DesignateForSending(TypeMeasure::E type);

    // ��� ���������, ������� ����� ������ � SCPI. ���� Count, �� �� ���� ������ ��������
    extern TypeMeasure::E sended;
};
