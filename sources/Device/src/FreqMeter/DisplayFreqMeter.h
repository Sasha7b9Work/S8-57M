// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


namespace DisplayFreqMeter
{
    // ��������� ��������
    void Update();

    // ������� ���������� ����������
    void DrawDebugInfo();
};


namespace ProgressBarFreqMeter
{
    void Draw(int x, int y);
};
