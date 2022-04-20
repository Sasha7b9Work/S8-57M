// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Chan
{
    enum E
    {
        A,
        B,
        Number
    } value;
    operator uint8() const { return static_cast<uint8>(value); }
};
