// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


// ����� �������� ���������, ������� ������������ ��� ���������� � �� ���������������� ��� ���������� �������


#define NRST_NUM_SMOOTH_FOR_RAND    (setNRST.numSmoothForRand)
#define NRST_CORRECTION_TIME        (setNRST.correctionTime)
#define NRST_ENUM_GATE_MAX          (setNRST.enumGateMax)
#define NRST_ENUM_GATE_MIN          (setNRST.enumGateMin)
#define NRST_EX_SHIFT(chan, range)  (setNRST.exShift[chan][range])
#define NRST_EX_STRETCH(chan)       (setNRST.exStretch[chan])
#define NRST_ENUM_AVERAGE_RAND      (setNRST.enumAverageRand)


struct SettingsNRST
{ //-V802
    /*
        �������� !!! ��� ���������� ������ ������ ��������� ������ ���� ���������
    */

    uint   size;                                    // ������ ���������. �� ���� ��� �������� �� ������ ����� ����������, ��� ��������� ����� - ����� ����� �������� 0xFFFFFFFF
    int16  numSmoothForRand;                       // ����� ����� ��� ����������� ������ � �������������.
    int16  correctionTime;                         // ����������� ��������� �������.
    int16  enumGateMax;                            // ����������� ����� � ������������� ������
    int16  enumGateMin;                            // ����������� ����� � ������������� �����
    int8   exShift[Ch::Count][Range::Count];     // �������������� ��������
    float  exStretch[Ch::Count];                 // �������������� �������� �� ������
    int8   enumAverageRand;                        // ���������� �������������� ���������� � �������������
    // ������������� ��� ���������. ��������� ���������� ������� �������� � ROM � �� �������� � ������, ���� ��������� ����. ���� �������� ���� - ������������� ���������� �� ���������
    void Init();
    // ���������� �������� � ROM. ���������� ������ ���� ��������� � setNRST �� ��������� � ����������� � ROM
    void Save();

    void Reset();

    bool operator!=(const SettingsNRST &rhs);

    // �������� �������������� ��������� ���
    void SetExtraShift(int8 shift);

    // �������� �������������� �������� ���
    void ResetExtraStretch();
};


extern SettingsNRST setNRST;
