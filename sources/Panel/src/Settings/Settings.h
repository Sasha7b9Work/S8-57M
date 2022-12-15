// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define BACKGROUND_BLACK true


struct Resolution
{
    enum E
    {
        Full,   // Полное разрешение 640х480
        Half    // Половинное разрешение 320х240
    };
    static E value;
    static void Set(E v) { value = v; }
    static bool IsFull() { return value == Full; }
};
