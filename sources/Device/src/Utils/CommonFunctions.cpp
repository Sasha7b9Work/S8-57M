#include "defines.h"
#include "log.h"
#include "CommonFunctions.h"
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <cstdio>



void CF::LogBufferU8_HEX(const uint8 *buffer, int num) //-V2506
{
    char *message = static_cast<char *>(malloc(static_cast<uint>(num * 3))); //-V2511

    if (message == nullptr)
    {
        return;
    }

    message[0] = 0;

    char value[10];

    for (int i = 0; i < num; ++i)
    {
        std::sprintf(value, "%02x ", buffer[i]);
        std::strcat(message, value); //-V2513
    }

    message[num * 3 - 1] = 0;

    LOG_WRITE(message);

    free(message); //-V2511
}


void CF::LogBufferU8_DEC(const uint8 *buffer, int num) //-V2506
{
    char *message = static_cast<char *>(malloc(static_cast<uint>(num * 3))); //-V2511

    if (message == nullptr)
    {
        LOG_WRITE("��� ������");
        return;
    }

    message[0] = 0;

    char value[10];

    for (int i = 0; i < num; ++i)
    {
        std::sprintf(value, "%03d ", buffer[i]);
        std::strcat(message, value); //-V2513
    }

    message[num * 3 - 1] = 0;

    LOG_WRITE(message);

    free(message); //-V2511
}


int CF::NumWords(char *string)
{
    int num = 0;

    do
    {
        if (*string != ' ' && *string != 0)             // ���� ��������� ������ - �� ������
        {
            ++num;                                      // �� ����� ��������� �����
            while (*string != ' ' && *string != 0)      // �������� ������� ������ �����
            {
                ++string;
            }
        }
        while (*string == ' ')                          // �������� �������
        {
            ++string;
        }
    } while(*string);

    return num;
}


char *CF::GetWord(char *string, int n, char *out, int size) //-V2506
{
    if (n >= NumWords(string))
    {
        return 0;
    }

    // ������� ������ �������� �����
    int currentWord = 0;
    do
    {
        if (*string != ' ')                         // ���� ������� ������ - �� ������, �� ����� ����� �����
        {
            if (currentWord == n)                   // ���� ������� ����� - ����
            {
                break;                              // �� ������� - string ���������� �� ������� �����
            }
            ++currentWord;
            while (*string != ' ' && *string != 0)     // �������� ����� ����� �����
            {
                ++string;
            }
        }
        while (*string == ' ')
        {
            ++string;                   // �������� �������
        }
    } while(*string);

    // ������� ����� �������� �������
    char *end = string;

    while (*end != ' ' && *end != 0)
    {
        ++end;                          // ���� ����� �����
    }

    int length = end - string;

    if (length + 1 > size)
    {
        return reinterpret_cast<char *>(0xffffffffU);   // �� ������ ����� � �������� ������ - ������� � ��������������� ����� //-V566
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = string[i];
    }
    out[length] = 0;

    return out;
}



BufferMax5::BufferMax5()
{
    Clear();
}


void BufferMax5::Clear()
{
    numElements = 0;
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;
}


void BufferMax5::Push(uint16 value)
{
    if (numElements < 5)
    {
        buffer[numElements++] = value;
    }
    else
    {
        if (value > buffer[0])
        {
            buffer[0] = value;
            std::sort(&buffer[0], &buffer[5]);
        }
    }
}


uint16 BufferMax5::GetValue(int index) //-V2506
{
    if (index >= numElements)
    {
        return 0xffff;
    }
    return buffer[index];
}
