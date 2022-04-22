// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


// »спользуетс€ дл€ управлени€ €ркостью диспле€
struct HAL_DAC2
{
    static void Init();
    // «начение value от 0 до 100
    static void SetValue(int value);
};


struct HAL_BUS
{
    static void Init();

    static void Update();

    struct Panel
    {
        static void Send(const uint8* data, int size);
    };
};


class HAL_LTDC
{
    friend class Display;

private:

    static void Init(uint front);

public:

    static void SetColors(uint *clut, uint numColors);
    static void CopyImage(uint8 *image, int x, int y, int width, int height);
};
