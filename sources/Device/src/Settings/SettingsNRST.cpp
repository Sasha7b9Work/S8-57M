// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


SettingsNRST setNRST;


static const SettingsNRST defaultNRST =
{
    0,                                   // size
    0,                                   // numSmoothForRand
    0,                                   // correctionTime
    0,                                   // enum_gate_max
    0,                                   // enum_gate_min
    {{0}, {0}},                          // shiftADC
    {1.0F, 1.0F}                         // stretchADCtype
};


void SettingsNRST::Init()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    if(!saved ||                                    // ���� ��� ��������� ��������
       saved->size == static_cast<uint>(-1))        // ��� � ������ �� ����� ������ ��� �� ���� ������ ��������
    {
        setNRST = defaultNRST;                      // �� ��������� �������� �� ���������
    }
    else if(saved->size != sizeof(*this))           // ���� ������ ���������� �������� �� ����� ������� ����� ��������� (� �� �� ����� ���� ������)
    {
        setNRST = defaultNRST;                      // �� ��������� �������� �� ���������
        std::memcpy(&setNRST, saved, saved->size);  // �� ��������� �� ������ �� ��������, ��� ���
        size = sizeof(*this);
    }
    else
    {
        setNRST = *saved;                           // � ���� ����� ������� ��������� ��������� � ����� ���������
    }
}


void SettingsNRST::Save()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    size = sizeof(*this);

    if(!saved ||                        // ���� ��� ���������� ��������
       saved->size != size ||           // ��� ������� �������� �� ���������
       *saved != setNRST)               // ��� ��������� ���������� � ������� �� ���������
    {
        ROM::NRST::Save(&setNRST);      // �� ��������� ���������
    }
}


void SettingsNRST::Reset()
{
    setNRST = defaultNRST;
}


bool SettingsNRST::operator!=(const SettingsNRST &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0;
}


void SettingsNRST::SetExtraShift(int8 shift)
{
    for(int ch = 0; ch < Chan::Count; ch++)
    {
        for(int range = 0; range < Range::Count; range++)
        {
            NRST_EX_SHIFT(ch, range) = shift;
        }
    }
}


void SettingsNRST::ResetExtraStretch()
{
    NRST_EX_STRETCH(ChanA) = 1.0F;
    NRST_EX_STRETCH(ChanB) = 1.0F;
}
