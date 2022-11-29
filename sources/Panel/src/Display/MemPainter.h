// 2022/04/22 11:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "common/Communicator/Message_.h"


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

    // Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
}
