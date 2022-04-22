// 2022/04/22 11:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace MemPainter
{
    void BeginPaint(int width, int height);

    void SetPoint(int x, int y);

    void Fill();

    void EndPaint(int x, int y);
}
