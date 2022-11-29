// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Symbol5
{
    enum E
    {
        COUPLE_AC  = '\x91',
        COUPLE_DC  = '\x92',
        COUPLE_GND = '\x90'
    };
};

struct Symbol8
{
    enum E
    {
        GOVERNOR_SHIFT_3    = '\x0e',  // ������ ����������
        GOVERNOR_LEFT       = '\x80',
        GOVERNOR_RIGHT      = '\x81',
        TSHIFT_NORMAL       = '\x84',  // ����������� �������� �� �������
        TSHIFT_NORMAL_BACK  = '\x85',  // ��� ��� ����������� �������� �� �������
        TSHIFT_LEFT         = '\x86',  // ����������� �������� �� ������� ��� �������� �� ����� ������� ������
        TSHIFT_RIGHT        = '\x87',  // ����������� �������� �� ������� ��� �������� �� ������ ������� ������
        CROSS               = '\x88',  // ������ ������������� ������
        TPOS_1              = '\x8a',  // 2-� ����� ������� TPos
        TPOS_2              = '\x8b',  // 1-� ����� ������� TPos
        RSHIFT_NORMAL       = '\x93',  // ������ �������� �� ���������� ����������
        TRIGLEV_NORMAL      = '\x94',  // ������ ������� ������������� ����������
        TRIGLEV_LOWER       = '\x95',  // ������ ������� ������������� ���� �����
        TRIGLEV_ABOVE       = '\x96',  // ������ ������� ������������� ���� �����
        GOVERNOR_SHIFT_2    = '\x8c',  // ������ ����������
        GOVERNOR_SHIFT_0    = '\xaa',  // ������ ����������
        GOVERNOR_SHIFT_1    = '\xac',  // ������ ����������
        PLAY                = '\xae',  // ������ ������ ������ ������ ������
        ARROW_UP            = '\xb9',
        ARROW_DOWN          = '\x9d'
    };
};

struct SymbolUGO
{
    enum E
    {
        Count
    };
};

#ifdef DELETE
#undef DELETE
#endif

struct SymbolUGO2
{
    enum E
    {
        PEAK_DET_LEFT   = '\x12',   // ����� ����� ������ �������� ���������
        PEAK_DET_RIGHT  = '\x13',   // ������ ����� ������ �������� ���������
        ARROW_LEFT      = '\x20',
        DELETE          = '\x22',   // ����� ��-�����-�������
        INSERT          = '\x26',   // ����� ��-���-��������
        SAVE_TO_MEM     = '\x2c',
        ROM             = '\x40',   // �����������
        FLASH_DRIVE_BIG = '\x42',   // ������ ������
        FOLDER          = '\x46',   // �������
        LEAVE           = '\x48',   // ������ ������
        ENTER           = '\x4a',   // ������ "Enter"
        TRIANGLE_UP     = '\x4c',   // ������� ������������� �����
        TRIANGLE_DOWN   = '\x4e',   // ������� ������������� ����
        BIG_M           = '\x60',   // ������� ����� "�"
        SETTINGS        = '\x62',   // ������ ��������
        ARROW_RIGHT     = '\x64',
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',   // ������ ������ ����� ������
        ETHERNET        = '\x82',   // ������ Ethernet ����� ������
        USB             = '\x84',   // ������ USB ����� ������
        MATH_FUNC_MUL   = '\x8a',   // ����������-�������-���������
        TRIANGLE_LEFT   = '\x8c',   // ������� ������������� �����
        TRIANGLE_RIGHT  = '\x8e',   // ������� ������������� ������
        SELECT          = '\xa0'    // ������ ������
    };
};
