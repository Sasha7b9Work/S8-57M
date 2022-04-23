#pragma once


typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void AddData(uint data);

    // ���������� true, ���� ���� ���������
    bool Update();

    // ���������� ������ ������, ���������� ���������
    int BytesInBuffer();

    // ���������� ��������� �� ������ ������� �������������� ������
    uint8 *Buffer();
};
