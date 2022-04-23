#pragma once
#include "Hardware/HAL/HAL.h"


struct Command
{           //  ��������               0  |     1       |     2      |      3      |     4        |      5       |   6    |         ������
    enum E
    {
        /* 00 */    None,                   //    |             |            |             |              |              |        |
        /* 01 */    ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |
        /* 02 */    Display_Brightness,     // 02 | �������
        /* 04 */    AddToConsole,           // 04 | num symb    |  ..... symbols ....

        /* 05 */    Paint_DirectLine,       // ��������� "������" �����
                    //  ��� ������� [8] | ����� �������� � ������� (������� �� ����������) [8] | ����� ����� [8] | ���������� ����� [8] | ...

        /* 06 */    Paint_DifferentLine,    // ��������� "����������" �����
                    //  ��� ������� [8] | ����� ����� � ������� (������� �� ����������) [8] | �������� ����� �� ��������� � ������� ������ [8] | ���������� ����� [8] | ...

                    Count
    };
};


#define TESTER_NUM_POINTS 120


#define SIZE_STRING (320)


// ��������� ����� ��������� - ������� (1) ��� ������� (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)


template<int capacity>
class Message
{
private:

    uint8 buffer[capacity];

public:

    Message(Command::E command, uint8 data)
    {
        buffer[0] = (uint8)command;
        buffer[1] = data;
    }

    Message(Command::E command, uint8 byte1, uint8 byte2)
    {
        buffer[0] = (uint8)command;
        buffer[1] = byte1;
        buffer[2] = byte2;
    }

    void Transmit()
    {
        HAL_BUS::Panel::Send(buffer, capacity);
    }
};


template<int capacity>
class DynamicMessage
{
private:

    uint8 buffer[capacity];
    int size;

public:

    DynamicMessage(Command::E command) : size(0) //-V1077
    {
        PushByte((uint8)command);
    }

    void PushByte(uint8 byte)
    {
        if (size < capacity)
        {
            buffer[size++] = byte;
        }
    }

    void PushByte(int position, uint8 byte)
    {
        if (position < capacity)
        {
            buffer[position] = byte;
        }
    }

    void Transmit()
    {
        HAL_BUS::Panel::Send(buffer, size);
    }
};
