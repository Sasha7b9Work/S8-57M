// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"


namespace BufferButtons
{
    // ������� ���������� ��� �������, ������� � ���������� ������
    void Push(const KeyEvent &str);

    // ��������� ������� �� �������
    KeyEvent Extract();

    // ���������� true, ���� ������� ������� �����
    bool IsEmpty();

    inline void Clear() { while (!IsEmpty()) { Extract(); } }

    // ���������� ���������� �����������, ��������� ����� ���������� ������� ����������
    uint TimeAfterControlMS();
};
