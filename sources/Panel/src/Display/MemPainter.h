// 2022/04/22 11:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace MemPainter
{
    // Задаёт область на экране для рисования.
    // Запись вне области, очерченной этим прямогульником, запрещена
    void BeginPaint(int x, int y, int width, int height);

    void SetPoint(int x, int y);

    // Нарисовать горизонтальную линию
    void DrawHLine(int y, int x0, int x1);

    void Fill();

    void EndPaint();
}
