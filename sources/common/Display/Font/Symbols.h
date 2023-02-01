// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Font/Font.h"


#ifdef DELETE
#undef DELETE
#endif

struct SymbolUGO2
{
    enum E
    {
        BIG_M           = (uint8)'\x60',   // ������� ����� "�"
        TAB             = (uint8)'\x6e',
        FLASH_DRIVE     = (uint8)'\x80',   // ������ ������ ����� ������
        ETHERNET        = (uint8)'\x82',   // ������ Ethernet ����� ������
        USB             = (uint8)'\x84',   // ������ USB ����� ������
        MATH_FUNC_MUL   = (uint8)'\x8a'    // ����������-�������-���������
    };
};


class Char
{
public:
    explicit Char(uint8 ch, TypeFont::E font = TypeFont::Normal, int num = 1);
    int Draw(int x, int y, Color color = Color::Count) const;
    char ToChar() const { return (char)ch; }
protected:
    const uint8 ch;
    const TypeFont::E font;
    const int num_symbols;        // ����� ���� 1, 2, 4 ��� 10
private:
    void Draw2Symbols(int x, int y) const;
    void Draw4Symbols(int x, int y) const;
    void Draw10Symbols(int x, int y) const;
};


class SymbolUGO : public Char
{
public:
    enum E
    {
        RSHIFT_NORMAL,      // ������ �������� �� ���������� ����������
        TRIGLEV_NORMAL,     // ������ ������� ������������� ����������
        PLAY,               // ������ ������ ������ ������ ������
        GOVERNOR_SHIFT_0,   // ������ ����������
        GOVERNOR_SHIFT_1,
        GOVERNOR_SHIFT_2,
        GOVERNOR_SHIFT_3,
        GOVERNOR_LEFT,
        GOVERNOR_RIGHT,
        TSHIFT_NORMAL,      // ����������� �������� �� �������
        CROSS,              // ������ ������������� ������
        COUPLE_GND,
        COUPLE_AC,
        COUPLE_DC,
        FRONT_FALL,
        FRONT_RISE,
        FILTR_HI,
        FILTR_LOW,
        FILTR_FULL,
        START_MODE_AUTO,
        START_MODE_HAND,
        START_MODE_SINGLE,
        TPOS,
        F_T,
        ARROW_UP,
        ARROW_DOWN,
        TSHIFT_LEFT,        // ����������� �������� �� ������� ��� �������� �� ����� ������� ������
        TSHIFT_RIGHT,       // ����������� �������� �� ������� ��� �������� �� ������ ������� ������
        TSHIFT_NORMAL_BACK, // ��� ��� ����������� �������� �� �������
        TPOS_1,             // 2-� ����� ������� TPos
        TPOS_2,             // 1-� ����� ������� TPos
        TRIGLEV_LOWER,      // ������ ������� ������������� ���� �����
        TRIGLEV_ABOVE,      // ������ ������� ������������� ���� �����
        PEAK_DET_LEFT,      // ����� ����� ������ �������� ���������
        DELETE,             // ����� ��-�����-�������
        INSERT,             // ����� ��-���-��������
        ARROW_LEFT,
        SAVE_TO_MEM,
        ROM,                // �����������
        FLASH_DRIVE_BIG,    // ������ ������
        DELETE_2,
        FOLDER,             // �������
        LEAVE,              // ������ ������
        ENTER,              // ������ "Enter"
        TRIANGLE_UP,        // ������� ������������� �����
        TRIANGLE_DOWN,      // ������� ������������� ����
        SETTINGS,           // ������ ��������
        TRIANGLE_LEFT,      // ������� ������������� �����
        TRIANGLE_RIGHT,     // ������� ������������� ������
        SELECT,             // ������ ������
        ARROW_RIGHT,
        Count
    };

    SymbolUGO(E);

private:

    struct SymbolStruct
    {
        uint8 code;
        int  num_symbols;
    };

    static const SymbolStruct desc[Count];
};
