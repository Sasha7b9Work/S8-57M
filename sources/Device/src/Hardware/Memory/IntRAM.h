// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


namespace IntRAM
{
    // ��������� �� 16k ���� ������, ��������������� ��� ������ �����������
    uint16 *Averager32k(Ch::E);

    // ������ ��� ������ ������ � ������ �������������
    uint8 *DataRand(Ch::E);

    // ��������� �� ������ ����������� ������
    DataSettings *PrepareForP2P();
};
