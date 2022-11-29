#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


namespace HAL
{
    void ErrorHandler(const char *, int);
};


// ������������ ��� ���������� �������� �������
namespace HAL_DAC2
{
    void Init();
    // �������� value �� 0 �� 100
    void SetValue(int value);
};


// ����� � �������� ������
namespace HAL_BUS
{
    void Init();

    void SendToDevice(uint8 *data, uint size);

    void Update();
};
