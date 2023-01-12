// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Symbol
{
    Symbol(uint8 _symbol) : symbol(_symbol) {}

    Symbol(char _symbol) : symbol((uint8)_symbol) {}

    int Width() const
    {
        return 14;
    }

    uint8 symbol;

    bool operator==(const char rhs) const
    {
        return symbol == rhs;
    }
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
    struct FSymbol
    {
        uchar width;
        uchar bytes[8];
    };

    int _height;
    FSymbol symbols[256];

public:
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    // Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static int GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // Устанавливает количество пустых пикселей между символами.
    static void SetSpacing(int spacing = 1);
    static int GetLengthText(pchar);
    static int GetSpacing();
    static void SetMinWidth(uint8 width);

private:
    static const Font *fonts[TypeFont::Count];
    static const Font *font;
};
