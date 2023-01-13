// 2023/01/13 10:53:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Symbols.h"


const SymbolUGO::SymbolStruct SymbolUGO::desc[SymbolUGO::Count] =
{
    { 1, 1},    // RSHIFT_NORMAL
    { 2, 1},    // TRIGLEV_NORMAL
    { 3, 4},    // PLAY
    { 7, 1},    // GOVERNOR_SHIFT_0
    { 8, 1},    // GOVERNOR_SHIFT_1
    { 9, 1},    // GOVERNOR_SHIFT_2
    {10, 1}     // GOVERNOR_SHIFT_3
};


Char::Char(char _ch, TypeFont::E type, int num) : ch(_ch), font(type), num_symbols(num)
{

}


int Char::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int result = 0;

    Font::Set(font);

    if (num_symbols == 1)
    {
        String("%c", ch).Draw(x, y);

        result = x + Symbol(ch).Width() + 1;
    }
    else if (num_symbols == 2)
    {
        //        Draw2SymbolsInRect(x, y, color);
    }
    else if (num_symbols == 4)
    {
        Draw4SymbolsInRect(x, y);
    }
    else if (num_symbols == 10)
    {
        //        Draw10SymbolsInRect(x, y, color);
    }

    Font::Pop();

    return result;
}


void Char::Draw2SymbolsInRect(int x, int y)
{

}


void Char::Draw4SymbolsInRect(int x, int y)
{
    char symbol = ch;

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 2; i++)
        {
            String("%c", symbol++).Draw(x + i * 16, y + j * 16);
        }
    }
}


void Char::Draw10SymbolsInRect(int x, int y)
{
    for (char i = 0; i < 5; i++)
    {
        String("%c", ch + i).Draw(x + 8 * i, y);
        String("%c", ch + i + 16).Draw(x + 8 * i, y + 8);
    }
}
