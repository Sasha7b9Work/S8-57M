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
        _GOST28,
        _GOST72bold,    // Используется для вывода показаний мультиметра
        _OMEGA72,       // Используется в мультиметре для вывода обозначения ом. Имеет только один символ
        Count,
        None
    } value;
};

struct Font
{
    int _height;
    Symbol symbols[256];

public:
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    // Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
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
