/*
    ����� ������ � ������� �� ������, ��� �� ���� ������� RAM
*/

#include "defines.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"
#include <cstring>
#include <cstdlib>


#ifdef OLD_VERSION

#define BEGIN reinterpret_cast<uint>(IntRAM::BeginStorageRAM())
#define END   reinterpret_cast<uint>(IntRAM::EndStorageRAM())

#else

#define BEGIN reinterpret_cast<uint>(ExtRAM::Begin())
#define END   reinterpret_cast<uint>(ExtRAM::End())

#endif


int16 RAM::currentSignal = 0;
Packet *RAM::oldest = reinterpret_cast<Packet *>(BEGIN);
Packet *RAM::newest = nullptr;


bool RAM::needNewFrame = true;


// ���������� �� ������ dest. ���������� ����� ������� ����� ����� ������
static uint *WriteToRAM(uint *dest, const void *src, int size)
{
    HAL_BUS::ConfigureToFSMC();
    
    uint8 *address = reinterpret_cast<uint8 *>(dest);

    std::memcpy(address, src, static_cast<uint>(size));
    
    return reinterpret_cast<uint *>(address + size);
}


bool RAM::canTrace = false;


struct Packet
{
    // ����� ���������� ������, ����� "�������". ���� addrNext == 0x00000000, ���������� ������ ���, ���� ����� ����� �����
    uint addrNewest;

    bool IsValid()
    {
        return reinterpret_cast<uint8*>(this) >= ExtRAM::Begin() && reinterpret_cast<uint8*>(this) < ExtRAM::End();
    }

    void Trace(uint)
    {
        if(addrNewest == 0 && RAM::canTrace)
        {
            addrNewest = addrNewest; //-V570
        }

        //LOG_WRITE("%d %x", line, addrNewest);
    }

    // ��������� ������ �� ������ this. ���������� ��������� �� �����, ��������� �� ���
    void Pack(const DataSettings *ds)
    {
        DataSettings data = *ds;
        data.dataA = nullptr;
        data.dataB = nullptr;

        addrNewest = 0x0000000;                                                                         // ���������, ��� ��� ����� ��������� �����

        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));                           // �� ����� ������ ������� DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // ���������� DataSettings

        if (ds->enableA)                                                                                // ���������� ������ ������� ������
        {
            data.dataA = reinterpret_cast<uint8 *>(address);
            address = WriteToRAM(address, ds->dataA, ds->BytesInChannel());
        }

        if (ds->dataB)                                                                                  // ���������� ������ ������� ������
        {
            data.dataB = reinterpret_cast<uint8 *>(address);
            WriteToRAM(address, ds->dataB, ds->BytesInChannel());
        }

        std::memcpy(reinterpret_cast<uint *>(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // ���������� ����������������� ���������
    }
    // ����������� ����� ��� ���������� ������ � ������������ � ����������� ds
    void Prepare(DataSettings *ds)
    {
        int bytesInChannel = ds->BytesInChannel();

        addrNewest = 0x00000000;
        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));

        ds->dataA = nullptr;
        ds->dataB = nullptr;
        uint8 *addrData = reinterpret_cast<uint8 *>(reinterpret_cast<uint8 *>(address) + sizeof(DataSettings));

        if (ds->enableA)
        {
            ds->dataA = addrData;
            std::memset(addrData, VALUE::NONE, static_cast<uint>(bytesInChannel));
            addrData += bytesInChannel;
        }

        if (ds->enableB)
        {
            ds->dataB = addrData;
            std::memset(addrData, VALUE::NONE, static_cast<uint>(bytesInChannel));
        }

        WriteToRAM(address, ds, sizeof(DataSettings));
    }

    uint Address() const
    {
        return reinterpret_cast<uint>(this);
    };
    // ���������� ����� ������� ���������� �� ������� �����
    uint End() const
    {
        return Address() + Size();
    }
    // ���������� ������ ������, ����������� ��� �������� ������ � ����������� � ����������� ds
    static int NeedMemoryForPacedData(const DataSettings *ds)
    {
        uint size = sizeof(Packet) + sizeof(DataSettings) + ds->NeedMemoryForData();

        return static_cast<int>(size);
    }

    int Size() const
    {
        uint size = sizeof(Packet) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();

        return static_cast<int>(size);
    }

    DataSettings *GetDataSettings() const
    {
        uint address = Address() + sizeof(Packet);

        return reinterpret_cast<DataSettings *>(address);
    }
};


void RAM::Init()
{
    oldest = reinterpret_cast<Packet *>(BEGIN);
    newest = nullptr;
}


DataSettings *RAM::PrepareForNewData()
{
    if(OSCI_IN_MODE_RANDOMIZER &&           // � ������������� ������ ����� ���������� ��������� �� ��������� ��������� ������
       (NumberDatas() != 0) &&              // �� ������ ���� � ��������� ��� ����
       !needNewFrame)                       // � ���� ����� �� �������� ����������� ����� �����
    {
        return Get();
    }

    DataSettings ds;

    ds.Fill();

    ds.id = NumberDatas() ? Get()->id + 1 : 0;

    uint address = AllocateMemoryForPacket(&ds);    // ������� ����� ��� ������ ������ ������

    if (newest)
    {
        newest->addrNewest = address;               // ��������� ��� � �������� ������ ���������� ������ ��� �����������
    }

    newest = reinterpret_cast<Packet *>(address);   // ������������� ���� ����� � �������� ��������� ������

    newest->Prepare(&ds);                           // � ����������� ������

    DataSettings *result = Get();

    result->timeMS = TIME_MS;

    if(OSCI_IN_MODE_RANDOMIZER && 
       (NumberDatas() != 0))
    {
        if (OSCI_IN_MODE_RANDOMIZER && (SampleType::IsReal() || S_TRIG_START_MODE_IS_SINGLE))
        {
            result->Clear();
        }
        else
        {
            if (result->IsEquals(*Get(1)))
            {
                result->CopyDataFrom(Get(1));
            }
        }

        needNewFrame = false;
    }

    return result;
}


DataSettings *RAM::Get(int numFromEnd)
{
    int number = NumberDatas();
    
    if (numFromEnd + 1 > number)
    {
        return nullptr;
    }

    int counter = number - 1 - numFromEnd;

    Packet *packet = oldest;

    while (counter > 0)
    {
        packet = reinterpret_cast<Packet *>(packet->addrNewest);
        counter--;
    }

    return packet->GetDataSettings();
}


int RAM::NumberDatas()
{
    HAL_BUS::ConfigureToFSMC();

    if (newest == nullptr)
    {
        return 0;
    }

    if (oldest == nullptr)
    {
        return 1;
    }

    int result = 0;

    Packet *packet = oldest;

    while (packet != nullptr)
    {
        result++;

        packet = reinterpret_cast<Packet *>(packet->addrNewest);

//        if(packet != nullptr && !packet->IsValid())     // \todo �������� �������. ����� �����������, ������ ������ �������� � ���������
//        {
//            LOG_WRITE("invalid packet 0x%x", packet);
//            break;
//        }
    }

    return result;
}

uint RAM::AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // ��� ��� �� ����� ������
    {
        return BEGIN;
    }

    uint addrFirst = newest->End();                                         // �� ����� ������ ������ ���� ��������� ������
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // � ��� ��������� ���� ��������� ������

    if (addrLast > END)
    {
        addrLast = BEGIN + Packet::NeedMemoryForPacedData(ds);
        while (reinterpret_cast<uint>(oldest) < addrLast)
        {
            RemoveOldest();
        }
        return BEGIN;
    }
                                                                            
    if (newest > oldest)                                                    // ���������� ������� ���������� - ����� ����� ������ ����������� ����� ����� ������
    {                                                                       
        if (addrLast < END)                                                 // ���� ������� ������ ��� ������
        {
            return addrFirst;
        }
        else                                                                // ���� ������� ������ �� ������� ��� ���������� ������
        {
            AllocateMemoryFromBegin(Packet::NeedMemoryForPacedData(ds));    // �� ����������� � ������ ������ ����������� ����� ���� ��� �������� ������

            return BEGIN;
        }
    }

    if (newest < oldest)                                                    // ����� ������ ������ ����������� ����� ����� �����
    {
        while (addrLast >= reinterpret_cast<uint>(oldest))
        {
            RemoveOldest();
            if (oldest->Address() == BEGIN)
            {
                break;
            }
        }
    }

    return addrFirst;
}


void RAM::AllocateMemoryFromBegin(int size)
{
    while (oldest->Address() - BEGIN < static_cast<uint>(size))
    {
        RemoveOldest();
    }
}


void RAM::RemoveOldest()
{
    oldest = reinterpret_cast<Packet *>(oldest->addrNewest);
}


bool RAM::LastSettingsEqualsCurrent()
{
    if(NumberDatas() == 0)
    {
        return false;
    }

    return Get()->EqualsCurrentSettings();
}
