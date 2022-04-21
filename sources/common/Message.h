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
