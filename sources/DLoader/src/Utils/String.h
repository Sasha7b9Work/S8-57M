#pragma once
#include "Display/Colors.h"
#include "Utils/String.h"


struct TypeConversionString
{
    enum E
    {
        None,           // �������������� ������ �� ������������
        FirstUpper      // ������ ������ - � ������ ��������, ��������� - � ������
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(pchar format, ...);
    ~String();

    void Set(TypeConversionString::E conv, pchar format, ...);

    char *c_str() const;
    // ���������� ����� �� ������ � ������� �����������
    int Draw(int x, int y, Color color = Color::Count) const;

    static pchar  const _ERROR;

    void Free();

    void Append(pchar str);

    void Append(pchar str, uint numSymbols);

    void Append(char symbol);
    // ������� numSymbols �� ������ ������
    void RemoveFromBegin(uint numSymbols);

    void RemoveFromEnd();

    uint Size() const;

    char &operator[](uint i);

private:

    bool Allocate(uint size);

    char *buffer;
};
