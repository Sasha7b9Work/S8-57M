// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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


#define BEGIN ((uint)ExtRAM::Begin())
#define END   ((uint)ExtRAM::End())


int16 RAM::currentSignal = 0;
Packet *RAM::oldest = (Packet *)BEGIN;
Packet *RAM::newest = nullptr;


bool RAM::needNewFrame = true;


// ���������� �� ������ dest. ���������� ����� ������� ����� ����� ������
static uint *WriteToRAM(uint *dest, const void *src, int size)
{
    HAL_BUS::ConfigureToFSMC();
    
    uint8 *address = (uint8 *)dest;

    std::memcpy(address, src, (uint)(size));
    
    return (uint *)(address + size);
}


bool RAM::canTrace = false;


struct Packet
{
    // ����� ���������� ������, ����� "�������". ���� addrNext == 0x00000000, ���������� ������ ���, ���� ����� ����� �����
    uint addrNewest;

    bool IsValid()
    {
        return (uint8 *)this >= ExtRAM::Begin() && (uint8 *)this < ExtRAM::End();
    }

    void Trace(uint)
    {
        if(addrNewest == 0 && RAM::canTrace)
        {
            addrNewest = addrNewest; //-V570
        }
    }

    // ��������� ������ �� ������ this. ���������� ��������� �� �����, ��������� �� ���
    void Pack(const DataSettings *ds)
    {
        DataSettings data = *ds;
        data.ch_a = nullptr;
        data.ch_b = nullptr;

        addrNewest = 0x0000000;                                                                         // ���������, ��� ��� ����� ��������� �����

        uint *address = (uint *)(Address() + sizeof(Packet));                           // �� ����� ������ ������� DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // ���������� DataSettings

        if (ds->en_a)                                                                                // ���������� ������ ������� ������
        {
            data.ch_a = (uint8 *)address;
            address = WriteToRAM(address, ds->ch_a, ds->BytesInChannel());
        }

        if (ds->ch_b)                                                                                  // ���������� ������ ������� ������
        {
            data.ch_b = (uint8 *)address;
            WriteToRAM(address, ds->ch_b, ds->BytesInChannel());
        }

        std::memcpy((uint *)(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // ���������� ����������������� ���������
    }
    // ����������� ����� ��� ���������� ������ � ������������ � ����������� ds
    void Prepare(DataSettings *ds)
    {
        int bytesInChannel = ds->BytesInChannel();

        addrNewest = 0x00000000;
        uint *address = (uint *)(Address() + sizeof(Packet));

        ds->ch_a = nullptr;
        ds->ch_b = nullptr;
        uint8 *addrData = (uint8 *)((uint8 *)address + sizeof(DataSettings));

        if (ds->en_a)
        {
            ds->ch_a = addrData;
            std::memset(addrData, VALUE::NONE, (uint)(bytesInChannel));
            addrData += bytesInChannel;
        }

        if (ds->en_b)
        {
            ds->ch_b = addrData;
            std::memset(addrData, VALUE::NONE, (uint)(bytesInChannel));
        }

        WriteToRAM(address, ds, sizeof(DataSettings));
    }

    uint Address() const
    {
        return (uint)this;
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

        return (int)(size);
    }

    int Size() const
    {
        uint size = sizeof(Packet) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();

        return (int)(size);
    }

    DataSettings *GetDataSettings() const
    {
        uint address = Address() + sizeof(Packet);

        return (DataSettings *)address;
    }
};


void RAM::Init()
{
    oldest = (Packet *)BEGIN;
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

    newest = (Packet *)address;                     // ������������� ���� ����� � �������� ��������� ������

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
        packet = (Packet *)packet->addrNewest;
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

        packet = (Packet *)packet->addrNewest;

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
        while ((uint)oldest < addrLast)
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
        while (addrLast >= (uint)oldest)
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
    while (oldest->Address() - BEGIN < (uint)(size))
    {
        RemoveOldest();
    }
}


void RAM::RemoveOldest()
{
    oldest = (Packet *)oldest->addrNewest;
}


bool RAM::LastSettingsEqualsCurrent()
{
    if(NumberDatas() == 0)
    {
        return false;
    }

    DataSettings *last_data = Get();

    return last_data ? last_data->EqualsCurrentSettings() : false;
}
