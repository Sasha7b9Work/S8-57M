// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define BACKGROUND_BLACK true


struct Resolution
{
    enum E
    {
        Full,   // ������ ���������� 640�480
        Half    // ���������� ���������� 320�240
    };
    static E value;
    static void Set(E v) { value = v; }
    static bool IsFull() { return value == Full; }
};
