// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"



namespace Display
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    void Init();

    // Записать данные data количеством num_bytes в буфер, начиная со смещения offset
    void SetField(uint offset, uint8 *data, int num_bytes);

    uint Update();

    void Update1();

    uint8 *GetBuffer();

    uint8 *GetBufferEnd();

    extern LTDC_HandleTypeDef hltdc;
};

