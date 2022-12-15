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
{           //  номер байте            0  |     1       |     2      |      3      |     4        |      5       |   6    | Размер
    enum E                                              
    {                                                   
            None,                   //    |             |            |             |              |              |        |
            ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |         
            Paint_BeginScene,       // 02 | num_field   |            |             |              |              |        |
            Paint_EndScene,         // 03 |             |            |             |              |              |        |
            Paint_SetColor,         // 04 |   Color     |            |             |              |              |        |
            Paint_FillRegion,       // 05 |   x[0:7]    |  x[8:15]   |      y      | width[0:7]   | width[8:15]  | height |
            Paint_DrawText,         // 06 |   x[0:7]    |  x[8:15]   |      y      | num symbols  |              |        |
            Paint_SetPalette,       // 07 | numColor    | value[0:7] | value[8:15] | value[16:23] | value[24:31] |        |
            Paint_DrawRectangle,    // 08 |   x[0:7]    |  x[8:15]   |      y      | width[0:7]   | width[8:15]  | height |
            Paint_DrawVLine,        // 09 |   x[0:7]    |  x[8:15]   |      y0     |      y1      |              |        |
            Paint_DrawHLine,        // 10 |    y        |  x0[0:7]   |   x0[8:15]  |   x1[0:7]    |  x1[8:15]    |        |
            Paint_SetFont,          // 11 | typeFont    |            |             |              |              |        |
            Paint_SetPoint,         // 12 |   x[0:7]    |  x[8:15]   |      y      |              |              |        |
            Paint_DrawLine,         // 13 |  x0[0:7]    | x0[8:15]   |     y0      |   x1[0:7]    |   x1[8:15]   |   y1   |
            Display_Brightness,     // 15 | яркость
            Screen,                 // 16 |  numRow     |
            Paint_VPointLine,       // 17 |   x[0:7]    |  x[8:15]   |      y      |   delta      | count        |
            Paint_HPointLine,       // 18 |   x[0:7]    |  x[8:15]   |      y      |   delta      | count        |
            Paint_SetMinWidthFont,  // 19 |   width     |
            Paint_SetTextSpacing,   // 20 |   0 | 1     |
            AddToConsole,           // 21 | num symb    |  ..... symbols ....
            // Установка режима рисования : 0 - 640х480, 1 - 320х240
            SetModeDrawing,         // 22 |   mode      |
            Count
    };
};


#define SIZE_STRING (320)


// Извлекает режим рисования - линиями (1) или точками (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)
