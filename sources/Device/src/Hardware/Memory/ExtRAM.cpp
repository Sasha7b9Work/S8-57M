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
    return Begin() + SIZE_RAM; //-V2563
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


float ExtRAM::Test1()
{
    HAL_BUS::ConfigureToFSMC();

    int SIZE = 1024 * 512;

    int bad = 0;

    uint8 *address = Begin();

    for(int i = 0; i < SIZE; i++)
    {
        *address = static_cast<uint8>(i);

        uint8 out = *address;

        if(out != static_cast<uint8>(i))
        {
            bad++;
        }

        address++;
    }

    return bad * 100.0F / SIZE; //-V2564
}


float ExtRAM::Test2()
{
#define SIZE 1024

    int bad = 0;

    uint8 bufferIN[SIZE];
    uint8 bufferOUT[SIZE];
    std::memset(bufferOUT, 0, SIZE);

    for(int x = 0; x < SIZE; x++)
    {
        bufferIN[x] = static_cast<uint8>(std::rand());
    }

    uint8 *address = Begin() + (std::rand() % (500 * 1024)); //-V2563

    Write(bufferIN, SIZE, address);

    Read(bufferOUT, SIZE, address);

    for(int z = 0; z < SIZE; z++)
    {
        volatile uint8 in = bufferIN[z];
        volatile uint8 out = bufferOUT[z];

        if(in != out)
        {
            bad++;
        }
    }

    return (bad * 100.0F) / SIZE; //-V2564
}


float ExtRAM::TestTime(int sizekB) //-V2506
{
    float result = 0;

    for(int i = 0; i < sizekB; i++)
    {
        float time = TestTime1kB(Begin() + i * 1024); //-V2563

        if(time == -1.0F) //-V550 //-V2550
        {
            return -1.0F;
        }

        result += time;
    }

    return result;
}


float ExtRAM::TestTime1kB(uint8 *address) //-V2506
{
#define SIZE_BUFFER 1024

    uint8 data[SIZE_BUFFER];
    uint8 out[SIZE_BUFFER];

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        data[i] = static_cast<uint8>(std::rand());
    }

    uint start = Timer::TimeUS();

    Write(data, SIZE_BUFFER, address);

    Read(out, SIZE_BUFFER, address);

    float time = (Timer::TimeUS() - start) / 1e6F; //-V2564

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        if(data[i] != out[i])
        {
            return -1.0F;
        }
    }

    return time;
}
