// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Buffer.h"
#include "Utils/Stack.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cstring>
#include <cstdlib>

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif


// ���������� false, ���� ����� ���������� - ������ ���������.
static bool ChooseSymbols(pchar *string);
// ���������� false, ���� ����� ���������� - ������ ���������.
static bool ChooseSpaces(pchar *string);


bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint length = std::strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


char *SU::Db2String(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    std::strcat(bufferOut, Float(value).ToString(false, numDigits).c_str());
    std::strcat(bufferOut, "��");
    return bufferOut;
}


int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}


#define  SYMBOL(x) (*(*(x)))


static bool ChooseSymbols(pchar *string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}


static bool ChooseSpaces(pchar *string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL


bool SU::GetWord(pchar string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = (char *)(string);
            ChooseSymbols(&string);
            word->numSymbols = static_cast<int8>(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = (char)(std::toupper(*pointer));
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}


bool SU::WordEqualZeroString(Word *word, char* string)
{
    char *ch = string;
    char *w = word->address;

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}


bool SU::EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}


bool SU::EqualsStrings(uint8 *str1, pchar  const str2, int size)
{
    return EqualsStrings((void *)(str1), (char *)(str2), size);
}


bool SU::EqualsStrings(void *_str1, void *_str2, int size)
{
    char *str1 = (char *)(_str1);
    char *str2 = (char *)(_str2);

    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


bool SU::EqualsStrings(pchar str1, pchar str2)
{
    uint size = std::strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


int SU::FirstNotNumeral(char *buffer)
{
    int result = 0;

    while (*buffer++)
    {
        char symbol = *buffer;

        if ((symbol < 0x30 || symbol > 0x39) && symbol != '.')
        {
            break;
        }

        result++;
    }

    return result;
}


float SU::StringToFloat(pchar string)
{
    float result = 0.0F;

    Stack<int8> stack(20);

    while (*string)
    {
        char symbol = *string;
        if (symbol < 0x30 || symbol > 0x39)
        {
            break;
        }
        stack.Push(symbol & 0x0f);
        string++;
    }

    {
        int pow = 1;

        while (stack.Size() > 0)
        {
            result += static_cast<float>(pow) * stack.Pop();
            pow *= 10;
        }
    }

    // ������ � result ����� ����� �����

    if (*string == '.')
    {
        string++;

        float pow = 0.1F;

        while (*string)
        {
            char symbol = *string;
            if (symbol < 0x30 || symbol > 0x39)
            {
                break;
            }
            result += pow * (symbol & 0x0f);
            pow /= 10.0F;
            string++;
        }
    }


    return result;
}


char *SU::ToUpper(char *str)
{
    return ToUpper(str, (int)(std::strlen(str)));
}


char *SU::ToUpper(char *_str, int size)
{
    char *str = (char *)(_str);

    for(int i = 0; i < size; i++)
    {
        str[i] = (char)(std::toupper(str[i]));
    }

    return str;
}


char SU::ToUpper(char symbol)
{
    uint8 s = (uint8)(symbol);

    if(s == 0x40)        // �
    {
        return '\xc5';
    }
    else if(s >= 0x60 && s <= 0x7a)
    {
        return (char)(s - 0x20);
    }
    else if(s >= 0xf0)
    {
        return (char)(s - 0x20);
    }

    return symbol;
}


char SU::ToLower(char symbol)
{
    uint8 s = (uint8)(symbol);

    if(s >= 0x41 && s <= 0x5a)
    {
        return (char)(s + 0x20);
    }
    else if(s >= 0xc0 && s < 0xE0)
    {
        return (char)(s + 0x20);
    }

    return symbol;
}


#ifdef VS_KEIL

char *SU::DoubleToString(double)
{
    return nullptr;
}

#else

char *SU::DoubleToString(double value)
{
    static char result[100];

    std::stringstream stream;
    stream << std::fixed << std::setprecision(5) << value;

    strcpy_s(result, 90, stream.str().c_str());

    char *p = result;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    return result;
}

#endif


bool SU::StringToDouble(double *value, pchar str)
{
    static const int SIZE_BUFFER = 100;
    char buffer[SIZE_BUFFER];

    strcpy_s(buffer, SIZE_BUFFER - 1, str);

    char *p = buffer;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    char *end = nullptr;

    *value = std::strtod(buffer, &end);

    return (end != buffer);
}


bool SU::String2Int(pchar buffer, int *value, char **end)
{
    Buffer string((int)(std::strlen(buffer)) + 1);

    std::strcpy(string.DataChar(), buffer);

    *value = std::strtol(string.DataChar(), end, 10);

    if (*end == string.DataChar())
    {
        *end = (char *)(buffer);
    }
    else
    {
        *end = (char *)(buffer) + (*end - string.DataChar());
    }

    return (*end != (char *)(buffer));
}


#ifndef GUI

int strcpy_s(char *dest, uint dest_size, pchar src)
{
    if (std::strlen(src) + 1 < dest_size)
    {
        std::strcpy(dest, src);
        return 0;
    }

    return 1;
}

#endif

