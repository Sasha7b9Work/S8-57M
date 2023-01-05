// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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


struct PacketNRST
{
    uint size;
    
    // ���������� ����� ���������� �� ���� ������
    PacketNRST *Next();
    
    // ����� ������� ����� � ������
    uint Begin() { return (uint)this;  }
    
    // ���������� ����� ������� ����� �� ���� �������
    uint End()   { return (uint)Next(); }
    
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
    PacketNRST *CreatePacket() const;
    
    // ���������� ��������� �� ��������� ���������� � ������� �����
    PacketNRST *LastPacket() const;
    
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
    PacketNRST *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : (SettingsNRST *)last;
}


PacketNRST *SectorNRST::CreatePacket() const
{
    return (PacketNRST *)sector.address;
}


PacketNRST *SectorNRST::LastPacket() const
{
    PacketNRST *packet = CreatePacket();

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


bool PacketNRST::IsEmpty() const
{
    return (size == 0xFFFFFFFFU);
}


bool SectorNRST::SaveSettings(SettingsNRST *nrst) const
{
    PacketNRST *last = LastPacket();                        // ������� ��������� ���������� � ������� �����

    if(last == nullptr)                                     // ���� ������� ��� ����
    {
        last = (PacketNRST *)sector.address;                // �� �� ����� � ����� ������ �������

        return last->SaveSettings(nrst);                    // � ���� � ���������
    }

    return last->Next()->SaveSettings(nrst);                // ���� ������� ���� �� ���� ����� - ������ ������� ������ �� ���
}


bool PacketNRST::SaveSettings(SettingsNRST *nrst)
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


PacketNRST *PacketNRST::Next()
{
    if(size == 0xFFFFFFFFU)
    {
        return nullptr;
    }

    return (PacketNRST *)(Begin() + size);
}


void ROM::NRST::Erase()
{
    sector1.sector.Erase();
    sector2.sector.Erase();
}
