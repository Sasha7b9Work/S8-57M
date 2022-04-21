#pragma once
#include "common/Command.h"


typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void AddData(uint8 data);

    // ���������� true, ���� ���� ���������
    bool Update();

    // ���������� ������ ������, ���������� ���������
    int BytesInBuffer();

    // ���������� ��������� �� ������ ������� �������������� ������
    uint8 *Buffer();
};
