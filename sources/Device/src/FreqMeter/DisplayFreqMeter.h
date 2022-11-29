#pragma once
#include "FreqMeter/FreqMeter.h"


struct DisplayFreqMeter
{
    // ��������� ��������
    static void Update();

private:
    // ������� ���������� ����������
    static void DrawDebugInfo();

    static void DrawFrequencyMode(int x, int _y);

    static void DrawPeriodMode(int x, int y);

    static bool needSendToSCPI;
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
