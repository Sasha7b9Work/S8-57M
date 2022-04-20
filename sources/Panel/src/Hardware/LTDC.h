// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class LTDC_
{
friend class Display;

private:

    static void Init(uint front, uint back);

public:

    static void SetColors(uint *clut, uint numColors);

    static void ToggleBuffers();

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);
};
