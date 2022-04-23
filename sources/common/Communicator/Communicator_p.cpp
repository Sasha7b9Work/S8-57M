#include "defines.h"
#include "common/Communicator/Communicator_.h"
#include "Utils/Buffer.h"


namespace Communicator
{
    class DataBuffer : public Buffer<uint8, 4096>
    {
    public:

        // ���������� true, ���� ���� ����� ������������������
        bool ExistEndOfSequence()
        {
            if (Size() < 12)
            {
                return false;
            }
        }

        bool FindSequence(uint8 *sequency, int size)
        {
            if (Size() < size)
            {
                return false;
            }

            ���
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
