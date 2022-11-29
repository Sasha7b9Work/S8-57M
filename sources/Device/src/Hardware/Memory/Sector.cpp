#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Osci/DeviceSettings.h"


const DataSettings *PacketROM::UnPack() const //-V2506
{
    if (!IsValid() || !IsData())
    {
        return nullptr;
    }

    return const_cast<DataSettings *>(reinterpret_cast<const DataSettings *>(reinterpret_cast<const uint8 *>(this) + sizeof(PacketROM))); //-V2563 //-V2567
}


PacketROM *PacketROM::Next() const //-V2506
{
    if (IsFree())
    {
        return nullptr;
    }

    return reinterpret_cast<PacketROM *>(reinterpret_cast<uint8 *>(const_cast<PacketROM *>(this)) + size); //-V2563
}


int PacketROM::Size() const //-V2506
{
    if (IsFree())
    {
        return static_cast<int>((Sector::Number(Address())) - Address());
    }

    return size;
}


bool PacketROM::WriteToSector(const Sector *sector) const //-V2506
{
    if(UnPack() == nullptr)
    {
        return false;
    }

    const PacketROM *dest = sector->FirstPacket();

    while (dest && !dest->IsFree())
    {
        dest = dest->Next();
    }

    uint addressWrite = reinterpret_cast<uint>(dest);       // �� ����� ������ ����� ������������� ������ ������ //-V2571

    uint lastAddress = addressWrite + size;                 // � ��� ��������� ����� ������

    if (lastAddress > sector->End())
    {
        return false;
    }

    DataSettings ds = *UnPack(); //-V522

    ds.dataA = nullptr;
    ds.dataB = nullptr;

    if (ds.enableA)
    {
        ds.dataA = reinterpret_cast<uint8 *>(addressWrite) + sizeof(PacketROM) + sizeof(DataSettings); //-V2563 //-V2571
    }
    if (ds.enableB)
    {
        ds.dataB = reinterpret_cast<uint8 *>(addressWrite) + sizeof(PacketROM) + sizeof(DataSettings); //-V2563 //-V2571
        if (ds.enableA)
        {
            ds.dataB += ds.BytesInChannel(); //-V2563
        }
    }

    HAL_ROM::WriteBufferBytes(addressWrite, this, sizeof(PacketROM));

    addressWrite += sizeof(PacketROM);

    HAL_ROM::WriteBufferBytes(addressWrite, &ds, sizeof(DataSettings));

    addressWrite += sizeof(DataSettings);

    if (ds.enableA)
    {
        HAL_ROM::WriteBufferBytes(addressWrite, UnPack()->dataA, ds.BytesInChannel());
        addressWrite += ds.BytesInChannel();
    }

    if (ds.enableB)
    {
        HAL_ROM::WriteBufferBytes(addressWrite, UnPack()->dataB, ds.BytesInChannel());
    }

    return true;
}


void PacketROM::Erase() const
{
    uint data = STATE_ERASED;

    HAL_ROM::WriteBufferBytes(Address(), &data, 4);
}


int PacketROM::GetPackedSize(const DataSettings *ds)
{
    uint size = sizeof(PacketROM) +     // Packet
        sizeof(DataSettings) +          // DataSettings
        ds->NeedMemoryForData();        // data

    return static_cast<int>(size);
}


void PacketROM::Log() const
{
//    const DataSettings *ds = UnPack();
//
//    int numInROM = ds ? static_cast<int>(ds->numInROM) : -1;
//    uint address = Address();
//    int numSector = Sector::Number(Address());
//    PacketROM *next = Next();
//
//    LOG_WRITE("PacketROM : data %d, address 0x%x(sector %d), next 0x%x(sector %d)",
//        numInROM,
//        address,
//        numSector,
//        next,
//        Sector::Number(reinterpret_cast<uint>(Next())));
//
//    if (ds)
//    {
//        ds->Log();
//    }
//    else
//    {
//        LOG_WRITE("����� ���� ���� ����");
//    }
}


uint Sector::End() const
{
    return address + size;
}


bool Sector::ExistPackets() const
{
    return !FirstPacket()->IsFree();
}


static void WriteToROM(uint *address, const void *data, int size)
{
    HAL_ROM::WriteBufferBytes(*address, data, size);
    *address += size;
}


void Sector::TranslateAddressToROM(const DataSettings *ds, const PacketROM *packet)
{
    uint8 *addressData = reinterpret_cast<uint8 *>(packet->Address() + sizeof(PacketROM) + sizeof(DataSettings)); // �� ����� ������ ����� �������� ������ ������� �� ������������ ������� //-V2571

    if (ds->enableA)
    {
        const_cast<DataSettings *>(ds)->dataA = addressData; //-V2567
        addressData += ds->BytesInChannel(); //-V2563
    }

    if (ds->enableB)
    {
        const_cast<DataSettings *>(ds)->dataB = addressData; //-V2567
    }
}


const PacketROM *Sector::WriteData(uint numInROM, const DataSettings *ds) const //-V2506
{
    const PacketROM *packet = FirstFreePacket();

    if (packet == nullptr)
    {
        return nullptr;
    }

    uint recordAddress = packet->Address();

    uint end = End();

    uint sizePacket = sizeof(PacketROM);

    uint sizeDS = sizeof(DataSettings);

    uint sizeData = static_cast<uint>(ds->BytesInChannel() * 2);

    volatile uint needSpace = sizePacket + sizeDS + sizeData;

    needSpace = needSpace;

    volatile uint existSpace = end - recordAddress;

    existSpace = existSpace;

    if (packet->Address() + PacketROM::GetPackedSize(ds) > End() - 10)      // ���� ����� ��� ������ ������� �� ������� �������. ��� ����� - � ����� ������� ������ �������� ������ 4 �����,
    {                                                                    // ���������� 0xFFFFFFFF �� ��������� ������ �������� �� ����� �� ������� �������.
        return nullptr;
    }

    PacketROM record = { STATE_VALID, static_cast<uint16>(PacketROM::GetPackedSize(ds)), TYPE_DATA };

    WriteToROM(&recordAddress, &record, sizeof(record));

    const_cast<DataSettings *>(ds)->numInROM = numInROM; //-V2567

    uint8 *addressDataA = ds->dataA;   // �� ���� ������� �������� ������, ���������� ������. �������� �� ����� ���, ��� � ds ����� �������� ����� - �� ������� ������ ����� ��������� � ROM.
    uint8 *addressDataB = ds->dataB;   // �� ���� ������� �������� ������, ���������� ������. �������� �� ����� ���, ��� � ds ����� �������� ����� - �� ������� ������ ����� ��������� � ROM.

    TranslateAddressToROM(ds, packet);

    WriteToROM(&recordAddress, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&recordAddress, addressDataA, ds->BytesInChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&recordAddress, addressDataB, ds->BytesInChannel());
    }

    return packet;
}


const PacketROM *Sector::FindValidPacket(uint numInROM) const //-V2506
{
    const PacketROM *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            const DataSettings *ds = packet->UnPack();

            if (ds && ds->numInROM == numInROM)
            {
                for (int j = 0; j < ds->BytesInChannel(); j++)
                {
                    if (ds->enableA)
                    {
                        if (ds->dataA[j] != static_cast<uint8>(j)) //-V2563
                        {
                            return packet;
                        }
                    }
                    if (ds->enableB)
                    {
                        if (ds->dataB[j] != static_cast<uint8>(j)) //-V2563
                        {
                            return packet;
                        }
                    }
                }

                return packet;
            }
        }

        packet = packet->Next();
    }

    return nullptr;
}


const DataSettings *Sector::ReadData(uint numInROM) const //-V2506
{
    const PacketROM *packet = FindValidPacket(numInROM);

    if (packet)
    {
        return packet->UnPack();
    }

    return nullptr;
}


const PacketROM *Sector::DeleteData(uint numInROM) const //-V2506
{
    const PacketROM *packet = FindValidPacket(numInROM);

    if (packet)
    {
        packet->Erase();
        return packet;
    }

    return nullptr;
}


const PacketROM *Sector::FirstPacket() const
{
    return reinterpret_cast<const PacketROM *>(address); //-V2571
}


const PacketROM *Sector::FirstFreePacket() const //-V2506
{
    const PacketROM *packet = FirstPacket();

    while (packet)
    {
        if (packet->IsFree())
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


void Sector::GetDataInfo(bool existData[ROM::Data::MAX_NUM_SAVED_WAVES]) const
{
    const PacketROM *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            const DataSettings *ds = packet->UnPack();
            if (ds)
            {
                existData[ds->numInROM] = true;
            }
        }
        packet = packet->Next();
    }
}


uint Sector::GetNumberWornBytes() const
{
    uint result = 0;

    const PacketROM *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsErased())
        {
            result += packet->Size();
        }

        packet = packet->Next();
    }

    return result;
}


const PacketROM *Sector::GetFirstPacketWithData() const //-V2506
{
    const PacketROM *result = FirstPacket();

    while (result && !result->IsFree())
    {
        if (result->IsValid())
        {
            return result;
        }

        result = result->Next();
    }

    return nullptr;
}


int Sector::Number(uint address) //-V2506
{
    if (address < ADDR_SECTOR(0))
    {
        return -1;
    }

    for (int i = 0; i < Sector::Count; i++)
    {
        if (address < END_SECTOR(i))
        {
            return i;
        }
    }

    return -1;
}


const Sector &Sector::Get(uint address)
{
    return HAL_ROM::sectors[Number(address)];
}


const Sector &Sector::Get(Sector::E number)
{
    return HAL_ROM::sectors[number];
}


char *TypePacket(const PacketROM *packet) //-V2506
{
    if (packet == nullptr)
    {
        return "nullptr";
    }

    if (packet->IsErased())
    {
        return "erased";
    }

    if (packet->IsFree())
    {
        return "free";
    }

    if (packet->IsData())
    {
        return "data";
    }

    return "not determinated";
}

int NumberPacket(const PacketROM *packet) //-V2506
{
    const DataSettings *ds = packet->UnPack();

    if (ds == nullptr)
    {
        return -1;
    }

    return static_cast<int>(ds->numInROM);
}


void Sector::Log() const
{
//    LOG_WRITE("    ������ %d ����� 0x%x, ������ 0x%x", number, address, size);
//    LOG_WRITE("             #   �����  ������  ���������");
//
//    PacketROM *packet = reinterpret_cast<PacketROM *>(address);
//
//    static int counter = 0;
//    counter = 0;
//
//    while (packet)
//    {
//        LOG_WRITE("%2d : %6s %2d 0x%x 0x%4x 0x%x",
//            counter,
//            TypePacket(packet),
//            NumberPacket(packet),
//            packet->Address(),
//            packet->Size(),
//            packet->Next());
//
//        packet = packet->Next();
//        counter++;
//    }
}
