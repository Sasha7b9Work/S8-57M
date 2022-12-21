#pragma once
#include "defines.h"


#define DISPLAY_ADD_STRING(x) Display::AddStringToIndicating(x)


struct Display
{
    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    static void Init();
    static void Update();
    static bool IsRun();
    // \todo ��������. ����� ����������
//    static void SaveRow(int) {};
};
