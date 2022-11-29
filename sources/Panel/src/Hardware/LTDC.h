#pragma once



class LTDC_
{
public:
    static void Init(uint front, uint back);

    static void SetColors(uint *clut, uint numColors);

    static void ToggleBuffers();

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);
};
