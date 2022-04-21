#pragma once
#include "common/Command.h"


template<int size_buffer>
class Message
{
private:

    uint8 buffer[size_buffer];
    int size;

public:

    Message(Command::E command, uint8 data)
    {
        buffer[0] = (uint8)command;
        buffer[1] = data;
        size = 2;
    }

    void Transmit()
    {
        HAL_BUS::Panel::Send(buffer, size);
    }
};
