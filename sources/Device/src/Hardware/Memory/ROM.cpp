#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include <cstring>


#define READ_BYTE(address)  (*((uint8 *)address))
#define SIZE_RECORD         512                      // ������ ����� ������


struct Record;

/*
    � ������ ������� ����������� (128 * 1024 / SIZE_RECORD) �������
*/

struct SectorSet
{
    // ���������� ����� ������ ������ (����������, ������ �������)
    Record *FirstRecord();

    // ���������� ��������� �� ��������� ������ � ����������
    Record *LastSaved();

    // ���������� ��������� �� ��������� � record ������ ������ (�����, � ������� ����� �������� ���������). ���� ������� �� �������, ���������� nullptr
    Record *NextFree(Record *record);

    // ���������� ����� ������ ��������� ��� ���������� ������
    Record *FirstFree();

    // ���������� true, ���� ������ record ����������� ����� �������
    bool Contains(Record *record);

    // ��������� � ��������� ��������� ������
    void CheckAndCorrect();

    Sector sector;
};


struct Record
{
public:

    // ���������� ��������� �� ��������� ������ ������ (������, � ������� ����� ��������� ���������)
    Record *NextFree();

    // ���������� ��������� �� ��������� ������
    Record *Next();

    // ���������� true, ���� ������ �������� (� �� ��� ������ �� ���������, ������ ����� 0xFFFFFFFF)
    bool IsFree();

    // ���������� true, ���� � ������ ���-�� �������� (������ ����� �� ����� 0xFFFFFFFF � 0x00000000). ��� ����� ����������� ����
    bool IsSaved();

    // ������� ������. ��������� ������ ������. ��� ����� ������ � ������������ �������� (� ����, � ������� �� ��������� ����������� �����)
    void Erase();

    // ���������� true, ���� ����� ���������. ��� ���������� � �������� ������� :
    // 1 - ������ ����� (������ �����  0xFFFFFFFF)
    // 2 - ������ ����� (������ ����� 0x00000000)
    // 3 - ������ ������ ��������� � ����������� ����� ���������
    bool IsCorrect();

    // ���������� ������, �������� ����������� ������ ������
    SectorSet *GetSector();

    // ���������� ��������� �� ������ ������
    SectorSet *GetAnotherSector();

    // ���������� ��������� �� ������ ����� ������
    uint *FirstDowbleWord();

    Settings set;
};


static SectorSet sectorFirst = { Sector::Get(Sector::_10_SETTINGS_1) };
static SectorSet sectorSecond = { Sector::Get(Sector::_11_SETTINGS_2) };


// ���������� ��������� �� ��������� ���������� ������
static Record *LastSaved();


void ROM::Init()
{
    ROM::Settings::CheckMemory();
}


void ROM::Settings::Erase()
{
    sectorFirst.sector.Erase();
    sectorSecond.sector.Erase();
}


void ROM::Settings::Save()
{
    Record *record = nullptr;

    Record *lastSaved = LastSaved();

    if(lastSaved)
    {
        record = lastSaved->GetSector()->FirstFree();

        if(!record)
        {
            SectorSet *sector = lastSaved->GetAnotherSector();
            
            record = sector->FirstFree();

            if(!record)
            {
                sector->sector.Erase();

                record = sector->FirstFree();
            }
        }
    }
    else
    {
        record = sectorFirst.FirstFree();
    }

    set.number = lastSaved ? (lastSaved->set.number + 1) : 0;                // ���� ������ ������, �� ����� ����� ����� 0 = 0xFFFFFFFF + 1
    set.crc32 = set.CalcWriteCRC32();

    HAL_ROM::WriteBufferBytes(reinterpret_cast<uint>(record), &set, sizeof(set)); //-V2571
}


bool ROM::Settings::Load()
{
    Record *record = LastSaved();

    if(record)
    {
        std::memcpy(&set, &record->set, sizeof(record->set));
    }

    return (record != nullptr);
}


void ROM::Settings::CheckMemory()
{
    sectorFirst.CheckAndCorrect();
    sectorSecond.CheckAndCorrect();
}


String OTP::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = HAL_OTP::GetSerialNumber(buffer);
    return String(buffer);
}


bool OTP::SaveSerialNumber(char *servialNumber)
{
    return HAL_OTP::SaveSerialNumber(servialNumber);
}


static Record *LastSaved() //-V2506
{
    Record *saved1 = sectorFirst.LastSaved();
    Record *saved2 = sectorSecond.LastSaved();

    if (saved1 && saved2)
    {
        return (saved1->set.number > saved2->set.number) ? saved1 : saved2;
    }

    if (!saved1 && !saved2)
    {
        return nullptr;
    }

    if (!saved2)
    {
        return saved1;
    }

    return saved2;
}


Record *Record::NextFree()
{
    return GetSector()->NextFree(this);
}


Record *Record::Next()
{
    uint8 *addressThis = reinterpret_cast<uint8 *>(this);

    uint8 *addressNext = addressThis + SIZE_RECORD; //-V2563

    return reinterpret_cast<Record *>(addressNext);
}


bool Record::IsFree()
{
    return (set.crc32 == 0xFFFFFFFFU);
}


SectorSet *Record::GetSector() //-V2506
{
    if(sectorFirst.Contains(this))
    {
        return &sectorFirst;
    }

    if(sectorSecond.Contains(this))
    {
        return &sectorSecond;
    }

    return nullptr;
}


SectorSet *Record::GetAnotherSector() //-V2506
{
    if(sectorFirst.Contains(this))
    {
        return &sectorSecond;
    }

    if(sectorSecond.Contains(this))
    {
        return &sectorFirst;
    }

    return nullptr;
}


bool Record::IsSaved()
{
    return (set.crc32 != 0xFFFFFFFFU) && (set.crc32 != 0x00000000U);
}


uint *Record::FirstDowbleWord()
{
    return reinterpret_cast<uint *>(&set.crc32);
}


bool Record::IsCorrect() //-V2506
{
    uint *start = FirstDowbleWord() + 1; //-V2563
    uint *end = FirstDowbleWord() + SIZE_RECORD / 4; //-V2563

    if (set.crc32 == 0xFFFFFFFFU)
    {
        for (uint *address = start; address < end; address++)
        {
            if (*address != 0xFFFFFFFFU)
            {
                return false;
            }
        }

        return true;
    }
    else if (set.crc32 == 0x00000000) //-V2516
    {
        for (uint *address = start; address < end; address++)
        {
            if (*address != 0x00000000)
            {
                return false;
            }
        }

        return true;
    }
    
    return (set.crc32 == set.CalcWriteCRC32());
}


void Record::Erase()
{
    HAL_ROM::Fill(reinterpret_cast<uint>(this), 0, SIZE_RECORD); //-V2571
}


Record *SectorSet::FirstRecord()
{
    return reinterpret_cast<Record *>(sector.address); //-V2571
}


bool SectorSet::Contains(Record *record)
{
    uint addressRecord = reinterpret_cast<uint>(record); //-V2571

    return addressRecord >= sector.address && addressRecord < sector.End();
}


Record *SectorSet::NextFree(Record *record) //-V2506
{
    record = record->Next();

    while(Contains(record))
    {
        if(record->IsFree())
        {
            return record;
        }
        record = record->Next();
    }

    return nullptr;
}


Record *SectorSet::LastSaved()
{
    Record *result = nullptr;

    Record *record = FirstRecord();

    while(Contains(record))
    {
        if(record->IsSaved())
        {
            if(record->IsCorrect())
            {
                if(result == nullptr)
                {
                    result = record;
                }
                else
                {
                    if(record->set.number > result->set.number)
                    {
                        result = record;
                    }
                }
            }
            else
            {
                record->Erase();
            }
        }

        record = record->Next();
    }

    return result;
}


Record *SectorSet::FirstFree() //-V2506
{
    Record *record = FirstRecord();

    while(Contains(record))
    {
        if(record->IsFree())
        {
            return record;
        }

        record = record->Next();
    }

    return nullptr;
}


void SectorSet::CheckAndCorrect()
{
    Record *record = FirstRecord();

    while (Contains(record))
    {
        if (!record->IsCorrect())
        {
            record->Erase();
        }

        record = record->Next();
    }
}
