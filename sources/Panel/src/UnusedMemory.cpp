// 2022/11/29 16:34:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "UnusedMemory.h"


namespace UnusedMemory
{
    static const uint SIZE = 16 * 1024 + 822;
    static uint8 memory[SIZE];
}


void UnusedMemory::Init()
{
    for (int i = 0; i < SIZE; i++)
    {
        memory[i] = (uint8)(i / 2);
    }
}
