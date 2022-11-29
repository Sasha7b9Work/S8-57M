#pragma once
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"



struct Device
{
    struct Mode
    {
        enum E
        {
            Osci,
            Tester,
            Multimeter,
            Recorder,
            Count
        } value;
        explicit Mode(E v) : value(v) {};
    };

    static void Init();

    static void Update();

    static void Reset();
    // ������������� ����� ������
    static void SetMode(Mode::E mode);
    // ���������� ������� ����� ������
    static Mode::E CurrentMode();

    static bool InModeTester();

    static bool InModeMultimeter();

    static bool InModeRecorder();

    static bool InModeOsci();
};
