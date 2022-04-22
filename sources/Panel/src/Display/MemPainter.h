// 2022/04/22 11:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace MemPainter
{
    // Задаёт область на экране для рисования.
    // Запись вне области, очерченной этим прямогульником, запрещена
    void BeginPaint(int x, int y, int width, int height);

    void SetPoint(int x, int y);

    void DrawHLine(int y, int x1, int x2);

    void DrawVLine(int x, int x1, int y1);

    void Fill();

    void EndPaint();
}
