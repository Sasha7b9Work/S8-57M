#pragma once
#include "common/Command.h"


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

    DynamicMessage(Command::E command) : size(0)
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

    void Transmit()
    {
        HAL_BUS::Panel::Send(buffer, size);
    }
};
