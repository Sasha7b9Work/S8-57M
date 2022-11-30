// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


// ����� �������� ���������, ������� ������������ ��� ���������� � �� ���������������� ��� ���������� �������


#define NRST_NUM_SMOOTH_FOR_RAND    (setNRST._numSmoothForRand)
#define NRST_CORRECTION_TIME        (setNRST._correctionTime)
#define NRST_ENUM_GATE_MAX          (setNRST._enumGateMax)
#define NRST_ENUM_GATE_MIN          (setNRST._enumGateMin)
#define NRST_EX_SHIFT(chan, range)  (setNRST._exShift[chan][range])
#define NRST_EX_STRETCH(chan)       (setNRST._exStretch[chan])
#define NRST_ENUM_AVERAGE_RAND      (setNRST._enumAverageRand)


struct SettingsNRST
{ //-V802
    /*
        �������� !!! ��� ���������� ������ ������ ��������� ������ ���� ���������
    */

    uint   size;                                    // ������ ���������. �� ���� ��� �������� �� ������ ����� ����������, ��� ��������� ����� - ����� ����� �������� 0xFFFFFFFF
    int16  _numSmoothForRand;                       // ����� ����� ��� ����������� ������ � �������������.
    int16  _correctionTime;                         // ����������� ��������� �������.
    int16  _enumGateMax;                            // ����������� ����� � ������������� ������
    int16  _enumGateMin;                            // ����������� ����� � ������������� �����
    int8   _exShift[Chan::Count][Range::Count];     // �������������� ��������
    float  _exStretch[Chan::Count];                 // �������������� �������� �� ������
    int8   _enumAverageRand;                        // ���������� �������������� ���������� � �������������
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
