#include "defines.h"
#include "Tables.h"
#include "Display/Primitives.h"
#include "common/Display/Font/Font_d.h"


// ������� �������� �������� � ������������ ��� ����� ������
pString Tables::symbolsAlphaBet[0x48] =
{
    /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
    /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
    /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
    //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - ��� ���������� ����� �������� � ���� �������
    /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
    //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - ��� ����� ���������� �������� �������� � set.memory.fileNameMask
};


pString Tables::Get(int index)
{
    return symbolsAlphaBet[index];
}


int Tables::Size()
{
    return static_cast<int>(sizeof(symbolsAlphaBet));
}


ENumSignalsInSec::E Tables::ENumSignalsInSecToENUM(int numSignalsInSec) //-V2506
{
    if(numSignalsInSec == 1)
    {
        return ENumSignalsInSec::_1;
    }
    else if(numSignalsInSec == 2)
    {
        return ENumSignalsInSec::_2;
    }
    else if(numSignalsInSec == 5)
    {
        return ENumSignalsInSec::_5;
    }
    else if(numSignalsInSec == 10)
    {
        return ENumSignalsInSec::_10;
    }
    else if(numSignalsInSec == 25) //-V2516
    {
        return ENumSignalsInSec::_25;
    }

    return ENumSignalsInSec::_1;
}


const char *Tables_RangeNameFromValue(Range::E range)
{
    static const struct StructName { pString name; } names[Range::Count] =
    {
        "Range_2mV",
        "Range_5mV",
        "Range_10mV",
        "Range_20mV",
        "Range_50mV",
        "Range_100mV",
        "Range_200mV",
        "Range_500mV",
        "Range_1V",
        "Range_2V",
        "Range_5V",
        "Range_10V",
        "Range_20V"
    };

    pString name = names[range].name;

    return name;
}


void Tables::DrawStr(int index, int x, int y)
{
    const char *str = symbolsAlphaBet[index];
    if (index == S_MEM_INDEX_CUR_SYMBOL_MASK)
    {
        Region(DFont::GetLengthText(str), 9).Fill(x - 1, y, Color::FLASH_10);
    }

    String(symbolsAlphaBet[index]).Draw(x, y, (index == S_MEM_INDEX_CUR_SYMBOL_MASK) ? Color::FLASH_01 : Color::FILL);
}
