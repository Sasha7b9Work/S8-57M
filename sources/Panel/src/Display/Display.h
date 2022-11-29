// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"



namespace Display
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    void Init();

    void Update();

    void Update1();

    uint8 *GetBuffer();

    uint8 *GetBufferEnd();

    void ClearBuffer(Color);

    extern LTDC_HandleTypeDef hltdc;
};

