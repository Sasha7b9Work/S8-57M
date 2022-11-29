#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct FPGA
{
    friend struct ContextFreqMeter;
    friend struct ContextOsci;
    friend struct ContextRecorder;
    friend struct Randomizer;

    friend struct Bandwidth;
    friend struct Device;
    friend struct Range;
    friend struct RShift;
    friend struct TBase;
    friend struct TrigInput;
    friend struct TrigPolarity;
    friend struct TShift;
    friend struct TrigLevel;
    friend struct Osci;

    friend class Tester;

    static bool IsRunning();

    // ��������/��������� ����������.
    static void LoadCalibratorMode();

    static void GiveStart(uint16 pred, uint16 post);

    static const int MAX_NUM_POINTS = 8 * 1024;

    // �������������� ������ �������������
    static void ForcedStart();

    static void LoadRegUPR();

    struct Flag
    {
        static void Read(bool updateFreqMeter = true);

        static void Clear();
        static bool DataReady();
        static bool TrigReady();
        static bool Pred();
        static bool HoldOff();

        static bool FreqReady();
        static bool PeriodReady();
        static bool FreqOverflow();
        static bool PeriodOverflow();
        static bool FreqInProcess();
        static bool PeriodInProcess();
    };

private:

    static void Init();

    static void OnPressStart();
    
    static void Reset();

    static uint16 post;
    static uint16 pred;   

    static bool isRunning;
   
    // True, ���� ��� �������������� ������
    static bool forcedStart;
};
