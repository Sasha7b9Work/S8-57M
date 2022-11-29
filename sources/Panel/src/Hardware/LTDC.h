#pragma once



class LTDC_
{
public:
    static void Init(uint buffer);

    static void SetColors(uint *clut, uint numColors);

//    static void ToggleBuffers();

private:

    static void SetBuffer(uint buffer);
};
