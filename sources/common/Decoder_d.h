// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"



typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void Clear();

    void AddData(uint8 data);

    // ���������� true, ���� ���� ���������
    bool Update();

    // ���������� ������ ������, ���������� ���������
    int BytesInBuffer();

    // ���������� ��������� �� ������ ������� �������������� ������
    uint8 *Buffer();
};
