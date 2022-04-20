// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


// ������������ ��� ���������� �������� �������
struct HAL_DAC2
{
    static void Init();
    // �������� value �� 0 �� 100
    static void SetValue(int value);
};


struct HAL_BUS
{
    static void Init();

    static void SendToDevice(uint8 *data, uint size);

    static void Update();
};


class HAL_LTDC
{
    friend class Display;

private:

    static void Init(uint front, uint back);

public:

    static void SetColors(uint* clut, uint numColors);

    static void ToggleBuffers();

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);
};
