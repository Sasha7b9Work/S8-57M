// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct TypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        Count,
        None
    } value;
};

struct Font
{
    int _height;
    Symbol symbols[256];

public:
    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    // ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static int GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing = 1);
    static int GetLengthText(pchar);
    static int GetSpacing();
    static void SetMinWidth(uint8 width);

private:
    static const Font *fonts[TypeFont::Count];
    static const Font *font;
};
