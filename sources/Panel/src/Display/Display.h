#pragma once
#include "defines.h"



namespace Display
{
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    void Init();

    uint8 *GetBuffer();

    uint8 *GetBufferEnd();

//    void ToggleBuffers();

    extern LTDC_HandleTypeDef hltdc;
};

