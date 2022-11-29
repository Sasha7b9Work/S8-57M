#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


namespace HAL
{
    void ErrorHandler(const char *, int);
};


// »спользуетс€ дл€ управлени€ €ркостью диспле€
namespace HAL_DAC2
{
    void Init();
    // «начение value от 0 до 100
    void SetValue(int value);
};


// —в€зь с основной платой
namespace HAL_BUS
{
    void Init();

    void SendToDevice(uint8 *data, uint size);

    void Update();
};


namespace LTDC_
{
    void Init(uint buffer);

    void SetColors(uint *clut, uint numColors);
};
