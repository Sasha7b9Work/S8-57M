#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"


/*
    ������� �������� �����.
    ������� ����������� ������ ������.
    ����� ������ ��������� �������� (������������ �� ����, ��� � �� �� �������� ����� ��� �������� ��������� �������� ��� ��������� ����������),
    ����������� ��������� ������������ � ������ ������� �������.
    ����� ����������� ��������� ������ ������, ��������� ������ ������ � ������ ������������ � ����. ��������� ������ ������
*/


struct Packet
{
    uint size;
    
    // ���������� ����� ���������� �� ���� ������
    Packet *Next();
    
    // ����� ������� ����� � ������
    uint Begin() { return reinterpret_cast<uint>(this);  }
    
    // ���������� ����� ������� ����� �� ���� �������
    uint End()   { return reinterpret_cast<uint>(Next()); }
    
    // ���������� true, ���� � ������ ��� ������ (����� ��������� �� ��������� ���������� �������)
    bool IsEmpty() const;
    
    // ������� �������� � ����� ��������� � �������
    bool SaveSettings(SettingsNRST *nrst);
};


struct SectorNRST
{
    // ���������� ��������� �� ���������� ��������� ���� ������� ������� � nullptr � ��������� ������
    SettingsNRST *GetSaved() const;
    
    // ���������� ��������� �� �����, ������������� �� ������ �������
    Packet *CreatePacket() const;
    
    // ���������� ��������� �� ��������� ���������� � ������� �����
    Packet *LastPacket() const;
    
    // ������� �������� � ������ ��������� � �������
    bool SaveSettings(SettingsNRST *) const;

    const Sector &sector;
};


static const SectorNRST sector1 = { HAL_ROM::sectors[Sector::_03_NRST_1] };
static const SectorNRST sector2 = { HAL_ROM::sectors[Sector::_04_NRST_2] };


SettingsNRST *ROM::NRST::GetSaved()
{
    if(sector2.GetSaved())          // ���� �� ������ ������� ��������� ���������
    {
        return sector2.GetSaved();  // �� �� � ����������
    }

    return sector1.GetSaved();      // ����� ���������� ��������� �� ������� �������, ���� ������� �������
}


void ROM::NRST::Save(SettingsNRST *nrst)
{
    if(!sector1.SaveSettings(nrst))         // ������� �������� �������� � ������ ������
    {
        if(!sector2.SaveSettings(nrst))     // ����� �������� �������� �� ������ ������
        {
            sector1.sector.Erase();
            sector1.SaveSettings(nrst);
            sector2.sector.Erase();
        }
    }
}


SettingsNRST *SectorNRST::GetSaved() const
{
    Packet *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : reinterpret_cast<SettingsNRST *>(last);
}


Packet *SectorNRST::CreatePacket() const
{
    return reinterpret_cast<Packet *>(sector.address);
}


Packet *SectorNRST::LastPacket() const
{
    Packet *packet = CreatePacket();

    while(!packet->IsEmpty())
    {
        if(packet->End() >= sector.End() - 4)   // ���������, ��� ����� �� ��������� �� ������� �������. ���������� ������ �����, � ������� ���������� ����������, ��� ��������� ����� ������
        {
            return packet;
        }

        if(packet->Next()->IsEmpty())           // ���� ��������� ����� - ������, �� �� ����� ��� �����
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


bool Packet::IsEmpty() const
{
    return (size == 0xFFFFFFFFU);
}


bool SectorNRST::SaveSettings(SettingsNRST *nrst) const
{
    Packet *last = LastPacket();                            // ������� ��������� ���������� � ������� �����

    if(last == nullptr)                                     // ���� ������� ��� ����
    {
        last = reinterpret_cast<Packet *>(sector.address);  // �� �� ����� � ����� ������ �������

        return last->SaveSettings(nrst);                    // � ���� � ���������
    }

    return last->Next()->SaveSettings(nrst);                // ���� ������� ���� �� ���� ����� - ������ ������� ������ �� ���
}


bool Packet::SaveSettings(SettingsNRST *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if((Begin() < sector.address)   ||                      // ��������� �� ����� �� ������ �������
       !IsEmpty()                   ||                      // ���������� �� ��, ����� �� ����� ������ ������ �� ���� �������� ���
       (sizeof(SettingsNRST) + Begin() >= sector.End()))    // ��������� �� ����� �� ����� �������
    {
        return false;
    }

    nrst->size = sizeof(SettingsNRST);

    HAL_ROM::WriteBufferBytes(Begin(), nrst, sizeof(SettingsNRST));

    return true;
}


Packet *Packet::Next()
{
    if(size == 0xFFFFFFFFU)
    {
        return nullptr;
    }

    return reinterpret_cast<Packet *>(Begin() + size);
}


void ROM::NRST::Erase()
{
    sector1.sector.Erase();
    sector2.sector.Erase();
}
