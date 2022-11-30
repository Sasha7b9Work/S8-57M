// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


// ���� ���������
struct TypeMeasure
{
// ������������ ���������
#define S_MEAS_INDICATED(num)   (set.meas._measures[num])
#define S_MEAS_MARKED           (set.meas._marked)

    enum E
    {
        None,
        VoltageMax,
        VoltageMin,
        VoltagePic,
        VoltageMaxSteady,
        VoltageMinSteady,
        VoltageAmpl,
        VoltageAverage,
        VoltageRMS,
        VoltageVybrosPlus,
        VoltageVybrosMinus,
        Period,
        Freq,
        TimeNarastaniya,
        TimeSpada,
        DurationPlus,
        DurationMinus,
        SkvaznostPlus,
        SkvaznostMinus,
        DelayPlus,
        DelayMinus,
        PhazaPlus,
        PhazaMinus,
        Count
    };

    static String GetName(TypeMeasure::E type);
};


struct Measure
{
public:
    Measure(int r, int c) : row(r), col(c), t(TypeMeasure::Count) {};

    Measure(TypeMeasure::E type) : row(0), col(0), t(type) {};

    TypeMeasure::E GetType();

    String GetStringMeasure(Chan::E ch, char *buffer, int lenBuf);
    // ������������� �������� ��������� ��� ���������� ���������
    static void ChangeActive(int delta);
    // ���������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    static float CalculateCursorU(Chan::E ch, float posCurT);
    // ���������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    static float CalculateCursorT(Chan::E ch, float posCurU, int numCur);

    static void SetMarkerVoltage(Chan::E ch, int num, float value);

    static void SetMarkerTime(Chan::E ch, int num, int value);

    static char GetChar(TypeMeasure::E measure);

    static void ShortPressOnSmallButonMarker();
    // ���������� true, ���� ��������� �������� - ������� ������
    bool IsActive();

    String Name();

private:

    // ������� ��������
    static void SetActive(int row, int col);
    // ������ � �������, � ������� ��������� ������ ���������
    int row;
    // ������� � �������, � ������� ��������� ������ ���������
    int col;

    TypeMeasure::E t;
};
