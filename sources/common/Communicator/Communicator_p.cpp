#include "defines.h"
#include "common/Communicator/Communicator_.h"
#include "Utils/Buffer.h"


namespace Communicator
{
    Buffer<uint8, 4096> data;
}


void Communicator::AddData(uint byte)
{
    data.Append((uint8)byte);
}


void Communicator::Panel::Update()
{
}
