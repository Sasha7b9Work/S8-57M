// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"


struct BufferButtons
{
    // ������� ���������� ��� �������, ������� � ���������� ������
    static void Push(const KeyEvent &str);
    
    // ��������� ������� �� �������
    static KeyEvent Extract();
    
    // ���������� true, ���� ������� ������� �����
    static bool IsEmpty();

    static void Clear() { while (!IsEmpty()) { Extract(); } }
    
    // ���������� ���������� �����������, ��������� ����� ���������� ������� ����������
    static uint TimeAfterControlMS();
};
