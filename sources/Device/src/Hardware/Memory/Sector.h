#pragma once
#include "Hardware/Memory/ROM.h"


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U

#define SECTOR(i) HAL_ROM::sectors[i]
#define ADDR_SECTOR(i) (SECTOR(i).address)
#define SIZE_SECTOR(i) (SECTOR(i).size)
#define END_SECTOR(i) (ADDR_SECTOR(i) + SIZE_SECTOR(i))


struct Sector;


struct PacketROM
{
    uint  state;    // ��������� ������:
                    // 0xFFFFFFFF - � ����� ������ �� �������������
                    // 0x00000000 - ����� ����
                    // 0xFF00FF00 - ����������� �����
    uint16 size;    // ������ ������ - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // ��� ������

    bool IsData() const { return (state == STATE_VALID) && (type == TYPE_DATA); }
    bool IsFree() const { return (state == STATE_FREE); }
    bool IsErased() const { return (state == STATE_ERASED); }
    bool IsValid() const { return (state == STATE_VALID); }

    uint Address() const { return reinterpret_cast<uint>(this); };

    PacketROM *Next() const;
    
    const DataSettings *UnPack() const;
    
    int Size() const;
    
    // ������ ������� ������ ������ � ������ sector. � ������ ������� ���������� false
    bool WriteToSector(const Sector *sector) const;
    
    void Erase() const;
    
    // ���������� ���������� ����, ����������� ��� �������� ������
    static int GetPackedSize(const DataSettings *ds);

    void Log() const;
};


struct Sector
{
    enum E
    {
        _00_BOOT_1,     // \ ���������
        _01_BOOT_2,     // |
        _02_BOOT_3,     // /
        _03_NRST_1,     // \ �������������� ���������
        _04_NRST_2,     // / 
        _05_FIRM_1,     // \ 
        _06_FIRM_2,     // | �������� ��������
        _07_FIRM_3,     // /
        _08,
        _09,
        // ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
        _10_SETTINGS_1, // ���������
        _11_SETTINGS_2, // 
        _12,
        _13,
        _14,
        _15,
        _16,
        _17,
        _18,
        _19_DATA_1,     // \ 
        _20_DATA_2,     // | ���������� ������
        _21_DATA_3,     // | 
        _22_DATA_4,     // |
        _23_DATA_5,     // /
        Count
    } number;

    uint address;

    uint size;
    
    // ����� ������� ����� �� ��������
    uint End() const;

    void Erase() const;
    
    // true ��������, ��� � ������ �� ������� �� ���� �����
    bool ExistPackets() const;

    const PacketROM *FirstFreePacket() const;

    const PacketROM *WriteData(uint numInROM, const DataSettings *ds) const;

    const DataSettings *ReadData(uint numInROM) const;

    const PacketROM *DeleteData(uint numInROM) const;
    
    // �������� ���������� � ��������� � ������� ������
    void GetDataInfo(bool existData[ROM::Data::MAX_NUM_SAVED_WAVES]) const;
    
    // ���������� ��������� �� ������ �����
    const PacketROM *FirstPacket() const;

    const PacketROM *FindValidPacket(uint numInROM) const;
    
    // ���������� ���������� ����, ���������� ������� ��������
    uint GetNumberWornBytes() const;
    
    // ���������� ��������� �� ������ ����� � ������� �� ������� sector � nullptr, ���� ������ � ������� ���
    const PacketROM *GetFirstPacketWithData() const;
    
    // ���������� ����� �������, �������� ����������� address
    static int Number(uint address);
    
    // ���������� ������ �� ������, �������� ����������� ������ �����
    static const Sector &Get(uint address);

    static const Sector &Get(Sector::E number);

    void Log() const;
private:
    static void TranslateAddressToROM(const DataSettings *ds, const PacketROM *packet);
};
