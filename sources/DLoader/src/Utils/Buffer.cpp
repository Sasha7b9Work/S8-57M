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
    if (size > 0)
    {
        std::memset(data, value, (uint)size);
    }
}


void Buffer::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = (uint8 *)(std::malloc((uint)s));
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
    LogU8(data);
}


void Buffer::LogU8(uint8 * /*buffer*/)
{
    LOG_WRITE("%d %d %d %d %d %d %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
}
