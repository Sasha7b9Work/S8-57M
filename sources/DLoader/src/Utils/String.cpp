#include "defines.h"
#include "Display/Primitives.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


const char * const String::_ERROR = "---.---";


String::String() : buffer(nullptr)
{
    Set(TypeConversionString::None, "");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(std::strlen(rhs.c_str()) + 1))
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(const char *format, ...) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (format == nullptr)
    {
        return;
    }

    static const int SIZE = 500;
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2528
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
        std::strcpy(buffer, "����� ������� ���");
    }
    else if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
    else
    {
        // ����� ������
    }
}


void String::Set(TypeConversionString::E /*conv*/, const char *format, ...)
{
    Free();

    if(format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(buffer, "����� ������� ���");
        }
        else if(Allocate(std::strlen(buf) + 1))
        {
            std::strcpy(buffer, buf);
        }
        else
        {
            // ����� ������
        }
    }
}


void String::Append(const char *str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(old.Size() + std::strlen(str) + 1);

    std::strcpy(buffer, old.c_str());
    std::strcat(buffer, str);
}


void String::Append(const char *str, uint numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    uint size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str());
    std::memcpy(buffer + old.Size(), str, numSymbols);
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer); //-V2511
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer); //-V2511
        buffer = nullptr;
        Set(TypeConversionString::None, "");
    }
}


char *String::c_str() const
{
    return buffer;
}


bool String::Allocate(uint size)
{
    std::free(buffer); //-V2511
    buffer = static_cast<char *>(std::malloc(size)); //-V2511
    if (buffer)
    {
        return true;
    }

    return false;
}


int String::Draw(int x, int y, Color color) const
{
    color.SetAsCurrent();
    return Text(c_str()).Draw(x, y);
}


void String::RemoveFromBegin(uint numSymbols)
{
    if (std::strlen(buffer) == numSymbols)
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0';
    }
}


uint String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return std::strlen(buffer);
}


char &String::operator[](uint i)
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}
