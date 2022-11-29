// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls.h"
#include "defines.h"


namespace Keyboard
{
    void Init();

    void Update();

    // ���������� ������� RL ��� ������ ����������
    const int NUM_RL = 6;

    // ���������� ������� SL ��� ������ ����������
    const int NUM_SL = 8;

    // ��������� ��� ������ ����������
    const char *ControlName(Control control);
};
