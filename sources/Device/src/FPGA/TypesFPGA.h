// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct WR
{
    static uint8 *START;        // �����
    static uint8 *TBASE;        // ��������
    static uint8 *PRED_LO;      // ����������
    static uint8 *PRED_HI;
    static uint8 *POST_LO;      // �����������
    static uint8 *POST_HI;

    static uint8 *UPR;                      // ����������
#define BIT_UPR_RAND            0           // ���. ������������
#define BIT_UPR_PEAK            1           // ������� ��������
#define BIT_UPR_CALIBR_AC_DC    2           // ���������� AC/DC
#define BIT_UPR_CALIBR_ZERO     3           // ���������� 0/4�

    static uint8 *FREQMETER;                // ���������� ������������
    static uint8 *TRIG;                     // ���������� ��������������/��������
#define BIT_TRIG_POLARITY       0           // ���������� ����������� �������������
#define BIT_TRIG_ENABLED        1           // ��� ��������� ������������. 0 - ���, 1 - ����

    static uint8 *START_ADDR;               // \brief ����� ������. ����� ����������� �� ����� ������, ����� ������ ������ � 
                                            //  ������������� ������ (������� �� ���������� � ����������)
    static uint8 *RESET_COUNTER_FREQ;       // ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
    static uint8 *RESET_COUNTER_PERIOD;     // ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������

    static uint8 *TRIG_HOLD_ENABLE;         // ���������� ��������� �������������. "0" - ����, "1" - ���
    static uint8 *TRIG_HOLD_VALUE_LOW;      // ������� ���� �������� �������������
    static uint8 *TRIG_HOLD_VALUE_MID;      // �������� �������������� �� ������� N = 0 - (�������� / 100��). ������������ �������� - 1.5 ���
    static uint8 *TRIG_HOLD_VALUE_HI;       // ������� ���� �������� �������������
};

struct RD
{
    static uint8 *DATA_A;               // ������ ������� ������
    static uint8 *DATA_A_PEAK_MAX;      // ������ �������� ��������� ������� ������
    static uint8 *DATA_A_PEAK_MIN;
    static uint8 *DATA_B;               // ������ ������� ������
    static uint8 *DATA_B_PEAK_MAX;      // ������ ������� ��������� ������� ������
    static uint8 *DATA_B_PEAK_MIN;
    static uint8 *LAST_RECORD_LO;       // ����� ��������� ������
    static uint8 *LAST_RECORD_HI;
    static uint8 *FREQ_BYTE_0;          // ���������� �������� �������
    static uint8 *FREQ_BYTE_1;
    static uint8 *FREQ_BYTE_2;
    static uint8 *FREQ_BYTE_3;
    static uint8 *PERIOD_BYTE_0;        // ���������� �������� �������
    static uint8 *PERIOD_BYTE_1;
    static uint8 *PERIOD_BYTE_2;
    static uint8 *PERIOD_BYTE_3;
    static uint8 *FLAG_LO;              // ������� ���� �����
    static uint8 *FLAG_HI;              // ������� ���� �����
};

struct Flag
{
    enum E
    {
        _DATA_READY        = 0,  // ������ ������ ��� ���������� (��������� �������� ������������)
        _TRIG_READY        = 1,  // ���� ��������������
        _PRED              = 2,  // ���� 1, �� ���������� ��������, ����� ������ �������������� ������ (���������
        _P2P               = 3,  // 
        _FREQ_READY        = 4,  // ���� ���������� ��������� �������
        _PERIOD_READY      = 5,  // ���� ���������� ��������� �������
        _HOLD_OFF_FLAG     = 7,  // �������������� ������ ����� ������ ������ ����� ���� ���� � �������
        _FREQ_OVERFLOW     = 8,  // ������� ������������ �������� �������
        _PERIOD_OVERFLOW   = 9,  // ������� ������������ �������� �������
        _FREQ_IN_PROCESS   = 10, // ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
        _PERIOD_IN_PROCESS = 11  // ������������� � ������� �������� ��������, ��� ��� ������� ��������� - ������� ������� � �������
    };
};
