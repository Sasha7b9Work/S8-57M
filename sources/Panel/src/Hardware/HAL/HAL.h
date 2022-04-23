// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HAL
{
    void Init();
    void ErrorHandler(const char *, int);
};


// »спользуетс€ дл€ управлени€ €ркостью диспле€
namespace HAL_DAC2
{
    void Init();
    
    // «начение value от 0 до 100
    void SetValue(int value);
};


namespace HAL_BUS
{
    void Init();

    void Update();

    namespace Panel
    {
        void Send(const uint8* data, int size);
    };
};


struct HAL_LTDC
{
    static void Init();
    static void LoadPalette();
    static void CopyImage(uint8 *image, int x, int y, int width, int height);
};


#ifdef DEBUG
#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)
#else
#define ERROR_HANDLER()
#endif
