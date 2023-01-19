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
    {10, 1},    // GOVERNOR_SHIFT_3
    {11, 1},    // GOVERNOR_LEFT
    {12, 1},    // GOVERNOR_RIGHT
    {18, 1},    // CROSS
    {15, 1},    // TSHIFT_NORMAL
    {19, 1},    // COUPLE_GND
    {20, 1},    // COUPLE_AC
    {21, 1},    // COUPLE_DC
    {22, 1},    // FRONT_FALL
    {23, 1},    // FRONT_RISE
    {24, 2},    // FILTR_HI
    {26, 2},    // FILTR_LOW
    {28, 2},    // FILTR_FULL
    {30, 1},    // START_MODE_AUTO
    {31, 1},    // START_MODE_HAND
    {32, 1},    // START_MODE_SINGLE
    {33, 1},    // TPOS
    {34, 1},    // F_T
    {39, 1},    // ARROW_UP
    {40, 1},    // ARROW_DOWN
    {42, 1},    // TSHIFT_LEFT
    {43, 1},    // TSHIFT_RIGHT
    {44, 1},    // TSHIFT_NORMAL_BACK
    {45, 1},    // TPOS_1
    {46, 1},    // TPOS_2
    {47, 1},    // TRIGLEV_LOWER
    {48, 1},    // TRIGLEV_ABOVE
    {59, 2},    // PEAK_DET
    {64, 4},    // DELETE
    {68, 4},    // INSERT
    {72, 4},    // ARROW_LEFT
    {76, 4},    // SAVE_TO_MEM
    {84, 4},    // ROM
    {88, 4},    // FLASH_DRIVE_BIG
    {92, 4},    // DELETE_2
    {96, 4},    // FOLDER
    {100, 4},   // LEAVE
    {104, 4},   // ENTER
    {108, 4},   // TRIANGLE_UP
    {112, 4}    // TRIANGLE_DOWN
};


Char::Char(char _ch, TypeFont::E type, int num) : ch(_ch), font(type), num_symbols(num)
{

}


int Char::Draw(int x, int y, Color color) const
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
        Draw2Symbols(x, y);
    }
    else if (num_symbols == 4)
    {
        Draw4Symbols(x, y);
    }
    else if (num_symbols == 10)
    {
        //        Draw10SymbolsInRect(x, y, color);
    }

    Font::Pop();

    return result;
}


void Char::Draw2Symbols(int x, int y) const
{
    char symbol = ch;

    int width = Symbol(symbol).Width();

    for (int i = 0; i < 2; i++)
    {
        String("%c", symbol++).Draw(x + i * width, y);
    }
}


void Char::Draw4Symbols(int x, int y) const
{
    char symbol = ch;

    int width = Symbol(symbol).Width();

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 2; i++)
        {
            String("%c", symbol++).Draw(x + i * width, y + j * width);
        }
    }
}


void Char::Draw10Symbols(int x, int y) const
{
    for (char i = 0; i < 5; i++)
    {
        String("%c", ch + i).Draw(x + 8 * i, y);
        String("%c", ch + i + 16).Draw(x + 8 * i, y + 8);
    }
}
