// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Buffer.h"
#include <cstdlib>
#include <cstring>



Buffer::Buffer(int s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, static_cast<uint>(size));
    }
}


void Buffer::Free()
{
    std::free(data); //-V2511
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(static_cast<uint>(s))); //-V2511
        size = (data) ? s : 0;

        if(!data)
        {
            LOG_ERROR("��� ������");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}


void Buffer::Log()
{
    LogU8_10(data);
}


void Buffer::LogU8_10(const uint8 *buffer, int num_point)
{
    if (num_point > 0)
    {
        LOG_WRITE("%d : %d %d %d %d %d %d %d %d %d %d", num_point, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
    }
    else
    {
        LOG_WRITE("%d %d %d %d %d %d %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
    }
}


void Buffer::DeltaU8_10(const uint8 *buffer, int num_point)
{
    int min = MinU8_10(buffer);
    int max = MaxU8_10(buffer);

    int delta = max - min;

    if (num_point > 0)
    {
        LOG_WRITE("%d : detla %d, min %d, max %d", num_point, delta, min, max);
    }
    else
    {
        LOG_WRITE("delta %d, min %d, max %d", delta, min, max);
    }
}


uint8 Buffer::MinU8_10(const uint8 *buffer)
{
    uint8 min = 255;

    for (int i = 0; i < 10; i++)
    {
        if (*buffer < min)
        {
            min = *buffer;
        }
        buffer++;
    }

    return min;
}


uint8 Buffer::MaxU8_10(const uint8 *buffer)
{
    uint8 max = 0;

    for (int i = 0; i < 10; i++)
    {
        if (*buffer > max)
        {
            max = *buffer;
        }
        buffer++;
    }

    return max;
}
