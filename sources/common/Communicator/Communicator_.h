#pragma once


typedef bool(*pFuncBU8)(uint8);


namespace Communicator
{
    void AddData(uint data);

    namespace Device
    {
        // ���������� true, ���� ���� ���������
        bool Update();

        // ���������� ������ ������, ���������� ���������
        int BytesInBuffer();

        // ���������� ��������� �� ������ ������� �������������� ������
        uint8 *Buffer();
    }
};
