// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Font/Font.h"


struct DAdvancedFont
{
    DAdvancedFont() {};
    // Установить действующий шрифт
    DAdvancedFont(TypeFont::E);
    // Возвращает true, если в строке row есть пиксели
    bool RowNotEmpty(uint8 symbol, int row);
    // Возвращает ширину символа
    uint8 GetWidth(uint8 symbol);
    // Возвращает высоту символа
    uint8 GetHeight(uint8 symbol);
    // Возвращает высоту самого высокого символа
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static TypeFont::E currentType;
};
