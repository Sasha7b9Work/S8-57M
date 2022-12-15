// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstring>


/*
*   Координаты передаются так: coord = (x | (y << 10))
*                              x = (coord & 0xA)
*                              y = (coord >> 10)
*/


struct Point2        // от слова dimensions
{
    Point2(int x = 0, int y = 0)  { xy = (uint)(x | (y << 10)); Reset(); }
    // При приёме следующего байта добавляем его этой функцией
    void Append(uint8 byte)       { *pointer = byte; pointer++; }
    void Reset()                  { pointer = (uint8 *)&xy;;   }
    // Берём 10 последних бит
    int X() const                 { return (int)(xy & 0x3ff);  }
    int Width() const             { return X(); }
    int Y() const                 { return (int)(xy >> 10);    }
    int Height() const            { return Y(); }
    void Write(uint8 *dest) const { std::memcpy(dest, &xy, 3); }
private:
    uint xy;
    uint8 *pointer;
};


struct Command
{
    enum E
    {       //  номер байте            0  |     1       |     2      |      3      |     4        |      5       |   6    | Размер |
            None,                   //                                                                                             |
            ButtonPress,            // 01 |   Item      | TypePress  |                                                    |   3    |
            Paint_BeginScene,       // 02 | num_field   |                                                                 |   2    |
            Paint_EndScene,         // 03 |                                                                               |   1    |
            Paint_SetColor,         // 04 |   Color     |                                                                 |   2    |
            Paint_FillRegion,       // 05 |              coord Point2              |             size Point2              |   7    |
            Paint_DrawText,         // 06 |              coord Point2              | num symbols  |                       |   5    |
            Paint_SetPalette,       // 07 | numColor    | value[0:7] | value[8:15] | value[16:23] | value[24:31] |        |   6    |
            Paint_DrawRectangle,    // 08 |              coord Point2              |             size Point2              |   7    |
            Paint_DrawVLine,        // 09 |              x0 y0 Pont2               |              y1 Point2               |   7    |
            Paint_DrawHLine,        // 10 |              x0 y0 Pont2               |              x1 Point2               |   6    |
            Paint_SetFont,          // 11 | typeFont    |                                                                 |   2    |
            Paint_SetPoint,         // 12 |              coord Point2              |                                      |   4    |
            Paint_DrawLine,         // 13 |              coord0 Point2             |             coord1 Point2            |   7    |
            Display_Brightness,     // 15 | яркость                                                                       |   2    |
            Screen,                 // 16 |  numRow     |                                                                 |   2    |
            Paint_VPointLine,       // 17 |              coord Point2              |   delta      |     count    |        |   6    |
            Paint_HPointLine,       // 18 |              coord Point2              |   delta      |     count    |        |   6    |
            Paint_SetMinWidthFont,  // 19 |   width     |                                                                 |   2    |
            Paint_SetTextSpacing,   // 20 |   0 | 1     |                                                                 |   2    |
            AddToConsole,           // 21 | num symb    |  ..... symbols ....                                             |   X    |
            // Установка режима рисования : 0 - 640х480, 1 - 320х240
            SetModeDrawing,         // 22 |   mode      |                                                                 |   2    |
            Count
    };
};


#define SIZE_STRING (320)


// Извлекает режим рисования - линиями (1) или точками (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)
