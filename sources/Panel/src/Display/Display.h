// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"



class Display
{
public:
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static uint8 *GetBuffer();

    static uint8 *GetBufferEnd();

    static void ToggleBuffers();

private:
    // �����
    static uint8 *frontBuffer;
    // ������ �����. � �� ���������� ���������, � ����� ����������� ���������� �� frontBuffer
    static uint8 *backBuffer;
};


extern LTDC_HandleTypeDef hltdc;
