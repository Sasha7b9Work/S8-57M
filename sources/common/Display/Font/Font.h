// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct SymbolRow
{
    SymbolRow(uint16 r);
    bool NotEmpty() const   { return (row != 0); }
    bool BitIsExist(int bit) const;
private:
    uint16 row;
};

struct Symbol
{
    Symbol(uint8 _symbol) : symbol(_symbol) {}

    Symbol(char _symbol) : symbol((uint8)_symbol) {}

    bool operator==(const char rhs) const { return symbol == rhs; }

    bool operator>=(const char rhs) const { return symbol >= rhs; }

    bool operator<=(const char rhs) const { return symbol <= rhs; }

    int Width() const;

    SymbolRow GetRow(int row) const;

    uint8 symbol;
};


struct TypeFont
{
    enum E
    {
        Small,
        Normal,
        Diagram,    // ��������� �� �������� ������ �������������� ���������
        UGO,        // ������ ������
        Count,
        None
    } value;
};

struct Font
{
    struct FSymbol
    {
        uchar width;
        uchar bytes[8];
    };

    int _height;
    FSymbol symbols[256];

public:
    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    // ��������������� �����, ������ ������� ����� ��������� ������� SetCurrent()
    static void Pop();
    // ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing = 1);
    static int GetLengthText(pchar);
    static int GetSpacing();
    static void SetMinWidth(uint8 width);
    static int Height();

private:
    static const Font *fonts[TypeFont::Count];
};