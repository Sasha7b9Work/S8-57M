// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"
//#include "Settings/Settings.h"


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
};


namespace Beeper
{
    void ResetCount();

    int GetCount();


    void Init();

    // ���� ������� �� ������.
    void ButtonPress();

    // ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();

    // ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    void WaitForCompletion();

    void Beep(TypePress::E type);

    void Test();

    namespace Bell
    {
        void On();

        void Off();
    };
};
