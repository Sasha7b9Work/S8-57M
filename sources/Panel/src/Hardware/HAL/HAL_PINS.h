// 2022/04/26 09:17:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HAL_PINS
{
    void Init();
}


enum Port
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I
};


struct PinPin { enum E
{
    _0,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _11,
    _12,
    _13,
    _14,
    _15
}; };


struct PinMode {
    enum E
    {
        _Input,
        _Output,
        _LTDC
    };
};


struct Pin
{
    Pin(PinMode::E _mode, Port _port, PinPin::E _pin) : port(_port), pin(_pin), mode(_mode) { };
    void Init();
    void Write(int state);
    void Set();
    void Reset();
    uint Read();

private:
    Port port;
    PinPin::E  pin;
    PinMode::E mode;
};

