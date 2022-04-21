#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Hardware/Memory/ROM.h"


static const int NUM_SECTORS = 5;

// ��� ������� ������������� ��� �������� ������
static const Sector *sectors[5] =
{
    &SECTOR(Sector::_19_DATA_1),
    &SECTOR(Sector::_20_DATA_2),
    &SECTOR(Sector::_21_DATA_3),
    &SECTOR(Sector::_22_DATA_4),
    &SECTOR(Sector::_23_DATA_5)
};


// ����� �������� ������ - ���������������� �� �� �������� ����� �������, ����� �������� ��������� ���������� ��� ������ ��� �������
static void Compress();
// ���������� ���������� ��������� ��������� ��� ������ ��������
static int NumberFreeSectors();
// ���������� ��������� �� ������ ��������� ������
static const Sector *GetFirstFreeSector();
// ���������� ��������� �� �������� ������� ������ - ��� ������ ������ �������� ������ ����� �����
static const Sector *GetMostWornSector();
// ����������� ������ � ��������� �����
static void CopyDataToFreeSpace(const Sector *src);


void ROM::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = false;
    }

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->GetDataInfo(existData);
    }
}


const DataSettings *ROM::Data::Read(uint numInROM)
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const DataSettings *ds = sectors[i]->ReadData(numInROM);

        if (ds)
        {
            return ds;
        }
    }

    return nullptr;
}


void ROM::Data::Erase(uint numInROM)
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (sectors[i]->DeleteData(numInROM))
        {
            return;
        }
    }
}


void ROM::Data::Save(uint numInROM, const DataSettings *ds)
{
    Erase(numInROM);

    Compress();

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const Sector *sector = sectors[i];

        if (sector->ExistPackets())                 // ����� ������ � ������, � ������� ��� ������������� ������
        {
            if (sector->WriteData(numInROM, ds))
            {
                return;
            }
        }
    }

    // ���� ����� � �������� ������� �������� ���, ����� ���������� � ���������

    GetFirstFreeSector()->WriteData(numInROM, ds);
}


void ROM::Data::EraseAll()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->Erase();
    }
}


static void Compress()
{
    while (NumberFreeSectors() < 2)
    {
        const Sector *src = GetMostWornSector();

        CopyDataToFreeSpace(src);

        src->Erase();
    }
}


static int NumberFreeSectors()
{
    int result = 0;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const PacketROM *packet = sectors[i]->FirstPacket();

        if (packet->IsFree())
        {
            result++;
        }
    }

    return result;
}


static const Sector *GetFirstFreeSector()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const PacketROM *packet = sectors[i]->FirstPacket();

        if (packet->IsFree())
        {
            return sectors[i];
        }
    }

    return nullptr;
}


static const Sector *GetMostWornSector()
{
    uint numWornBytes = 0;               // ���������� ������ ���� 

    const Sector *sector = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        uint numBytes = sectors[i]->GetNumberWornBytes();

        if (numBytes > numWornBytes)
        {
            numWornBytes = numBytes;
            sector = sectors[i];
        }
    }

    return sector;
}


static void CopyDataToFreeSpace(const Sector *sectorSrc)
{
    const PacketROM *packet = sectorSrc->GetFirstPacketWithData();

    if (packet)
    {
        for (int i = 0; i < NUM_SECTORS; i++)
        {
            if (sectors[i] != sectorSrc)
            {
                while (packet && !packet->IsFree())
                {
                    if (packet->IsData())
                    {
                        if (!packet->WriteToSector(sectors[i]))
                        {
                            break;
                        }
                    }

                    packet = packet->Next();
                }
            }
        }
    }
}
