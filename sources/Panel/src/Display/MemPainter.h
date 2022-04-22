// 2022/04/22 11:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "common/Command.h"


namespace MemPainter
{
    // Задаёт область на экране для рисования.
    // Запись вне области, очерченной этим прямогульником, запрещена
    void BeginPaint(int x, int y, int width, int height);

    void Fill();

    void EndPaint();

    void SetPoint(int x, int y);

    void DrawHLine(int y, int x1, int x2);

    void DrawVLine(int x, int x1, int y1);

    void DrawVPointLine(int x, int y, int delta, int count);

    void DrawHPointLine(int x, int y, int delta, int count);

    void DrawRectangle(int x, int y, int width, int height);

    void FillRegion(int x, int y, int width, int height);

    void DrawLine(int x1, int y1, int x2, int y2);

    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    // Нарисовать данные тестер-компонента. В младшем бите - линиями или точками рисовать. В старшей тетраде - количество усреднений
    void DrawTesterData(uint8 mode, Color color, const uint16 x[TESTER_NUM_POINTS], const uint8 y[TESTER_NUM_POINTS]);
}
