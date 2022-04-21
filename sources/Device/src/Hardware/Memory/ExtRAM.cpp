#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtRAM.h"
#include <cstdlib>
#include <cstring>


#define SIZE_RAM (512 * 1024)


#ifdef GUI
static uint8 ram[SIZE_RAM];
#endif


uint8 *ExtRAM::Begin()
{
#ifdef GUI
    return ram;
#else
    return reinterpret_cast<uint8 *>(0x68000000U); //-V566 //-V2571
#endif
}


uint8 *ExtRAM::End()
{
    return Begin() + SIZE_RAM;
}


void ExtRAM::Write(const uint8 *buffer, int size, uint8 *address)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    std::memcpy(address, buffer, static_cast<uint>(size));
}


void ExtRAM::Read(uint8 *buffer, int size, const uint8 *address)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    std::memcpy(buffer, address, static_cast<uint>(size));
}


void ExtRAM::Fill(uint8 *begin, uint8 value, int size)
{
    if(begin == 0)
    {
        begin = Begin();
        value = 0;
        size = SIZE_RAM;
    }

    HAL_BUS_CONFIGURE_TO_FSMC();

    std::memset(begin, value, static_cast<uint>(size));
}
