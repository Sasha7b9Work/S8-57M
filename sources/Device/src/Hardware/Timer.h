// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"

#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


// ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32)
 // � ����� ������� 120.000.000 ����� ��� �8-53 � 90.000.000 ����� ��� �8-54.
 // ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
#undef TIME_TICKS
#undef TIME_MS
#define TIME_TICKS HAL_TIM2::TimeTicks()
#define TIME_US    HAL_TIM2::TimeUS()
#define TIME_MS    HAL::TimeMS()


struct TypeTimer
{
    enum E
    {
        ShowLevelTrigLev,       // ����� �������������, ����� ���������� ��������� ����� ������ �������������
        ShowMessages,           // ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
        MenuAutoHide,           // ������ ��� ������� ������� ��������� ����.
        RShiftMarkersAutoHide,  // ������ ��� ������� ������� ��������� �������������� ������� �����.
        StopSound,              // ��������� ����
        StrNaviAutoHide,        // ������� ������ ��������� ����
        Display,                // ������ �� ������ ��������� ������
        RemoveAddFunction,      // ������ �� �������� �������������� ������� ���������
        USB,                    ///<
        Temp,
        Count
    };
};


namespace Timer
{
    void Init();

    void DeInit();

    // ��������� ������� timer ������� � ����� ������������
    void Set(TypeTimer::E type, pFuncVV func, uint dTms);

    void SetAndStartOnce(TypeTimer::E type, pFuncVV func, uint dTms);

    void SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms);

    void StartOnce(TypeTimer::E type);

    void Enable(TypeTimer::E type);

    void Disable(TypeTimer::E type);

    // ���������� true, ���� ������ � ������ ������ ���� ������
    bool IsRunning(TypeTimer::E type);

    void PauseOnTime(uint timeMS);

    void PauseOnTicks(uint numTicks);

    // ��������� ������� ��� ��������� ����� �������� �������
    void StartMultiMeasurement();

    // ������� ����������� ����� ������������ �������� � ����� for
    void PauseOnOPS(uint ops);

    bool IsBusy();

    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����
    void StartLogging();

    uint LogPointUS(char *name);

    uint LogPointMS(char *name);

    // ��������� �������. ���������� ������ �� ����������
    void ElapsedCallback();
};
