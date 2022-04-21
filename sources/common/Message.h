#pragma once
#include "common/Command.h"


template<int size_buffer>
class Message
{
private:
    uint8 buffer[size_buffer];
public:

    Message(Command::E, uint8);

    void Transmit();
};
