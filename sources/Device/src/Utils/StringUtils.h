// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once



struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


class SU
{
public:
    // ��� ������� ����� ����������� � �������� �������� �����.
    static bool GetWord(pchar string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, char* string);

    static char *Db2String(float value, int numDigits, char bufferOut[20]);
    // ���������� ��� ������. ����� �������� ������ �� str1
    static bool EqualsStrings(pchar str1, pchar str2);

    static bool EqualsStrings(void *str1, void *str2, int size);

    static bool EqualsStrings(uint8 *str1, pchar  const str2, int size);

    static bool EqualsZeroStrings(char *str1, char *str2);
    // ���������� ������� ������� �������, ������� �� �������� ������ ���� ������
    static int FirstNotNumeral(char *buffer);
    // ����������� ������ � float. �������������� ������� �� ������� ����������� �������
    static float StringToFloat(pchar string);

    static char *ToUpper(char *str, int size);

    static char *ToUpper(char *str);

    static char ToUpper(char symbol);

    static char ToLower(char symbol);

    static char *DoubleToString(double value);

    static bool StringToDouble(double *value, pchar buffer);

    static bool String2Int(pchar buffer, int *value, char **end);
};


#ifndef GUI

int strcpy_s(char *dest, uint dest_size, pchar src);

#endif
