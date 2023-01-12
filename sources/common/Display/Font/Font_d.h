// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DSymbol
{
    uchar width;
    uchar bytes[8];
};

struct DTypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        _GOST28,
        _GOST72bold,    // ������������ ��� ������ ��������� �����������
        Count,
        None
    };
};

struct DFont
{
    int _height;
    DSymbol symbols[256];

public:
    static int  GetLengthText(pString text);

    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const DTypeFont::E typeFont);

    static DTypeFont::E Current();

    // ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();

    static bool IsBig();

    static uint8 GetWidth(uint8 symbol);

    static uint8 GetWidth(char symbol);

    static uint8 GetHeight();

    static bool RowNotEmpty(uint8 symbol, int row);

    static bool BitIsExist(uint8 symbol, int row, int bit);

    static void SetMinWidth(uint8 width);

    // ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing = 1);

    // ���������� ���������� ������ �������� ����� ���������
    static int GetSpacing();

private:

    static const DFont *fonts[DTypeFont::Count];
    static const DFont *font;
};
