#include "defines.h"
#include "FPGA/FPGA.h"


// ����� ������ ���������� ������ � ���������� ������
static uint timePrevReadP2P = 0;


extern uint16 flagFPGA;


void FPGA::Flag::Read(bool updateFreqMeter)
{
    UNUSED_PARAMETER(updateFreqMeter);

    flagFPGA = 0xFFFF;
}
