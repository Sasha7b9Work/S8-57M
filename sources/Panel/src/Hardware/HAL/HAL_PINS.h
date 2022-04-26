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
    Pin(PinMode::E _mode, Port _port, int _pin) : port(_port), pin(_pin), mode(_mode) { };
    void Init();
    void Write(int state);
    void Set();
    void Reset();
    uint Read();

private:
    Port port;
    int  pin;
    PinMode::E mode;
};

