//
//  � ���� ����� ���������� ������� ��������� ������������
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Recorder/Recorder_win.h"
#include "Settings/Settings.h"
#include <cmath>


bool RecorderHAL::ReadyPoint() //-V2506
{
    static uint timeLastRead = 0;   // ����� ����������� ������ �����

    static const uint delta[Recorder::ScaleX::Count] =
    {
        100 / 20,   // 0.1�
        200 / 20,   // 0.2�
        500 / 20,   // 0.5�
        1000 / 20,  // 1�
        2000 / 20,  // 2�
        5000 / 20,  // 5�
        10000 / 20  // 10�
    };

    if (TIME_MS - timeLastRead >= delta[S_REC_SCALE_X])
    {
        timeLastRead = TIME_MS;
        return true;
    }

    return false;
}


uint8 RecorderHAL::ReadData(Ch::E ch)
{
    static float start[2] = { 0.0F, 250.0F };

    float amplitude = 120;

    return static_cast<uint8>(VALUE::AVE + std::sinf(start[ch] + TIME_MS / 500.0F) * amplitude); //-V2564
}
