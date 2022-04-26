#include "defines.h"
#include "common/Communicator/Communicator_.h"
#include "Utils/Buffer.h"
#include <cstring>


namespace Communicator
{
    class DataBuffer : public Buffer<uint8, 4096>
    {
    public:

        // Возвращает true, если есть конец последовательности
        bool ExistEndOfSequence()
        {
            if (Size() < 12)
            {
                return false;
            }

            static const uint8 marker[4] = { 0xff, 0xff, 0xff, 0xff };

            uint8 *seq = FindSequence(marker, 4);

            if (!seq)
            {
                return false;
            }

            seq += 4;

            return false;
        }

        // Возвращает адрес первого символа
        uint8 *FindSequence(const uint8 *sequency, int sz)
        {
            if (Size() < sz)
            {
                return false;
            }

            uint8 *byte = First();

            uint8 *last = Last() - sz;

            while (byte < last)
            {
                if (*sequency == *byte)
                {
                    if (std::memcmp((void *)(sequency + 1), byte + 1, sz - 1) == 0)
                    {
                        return byte;
                    }
                }
            }

            return nullptr;
        }
    };

    DataBuffer data;
}


void Communicator::AddData(uint byte)
{
    data.Append((uint8)byte);
}


void Communicator::Panel::Update()
{
    if (!data.ExistEndOfSequence())
    {
        if (data.Size() == data.Capacity())
        {
            data.Clear();
        }

        return;
    }
}
