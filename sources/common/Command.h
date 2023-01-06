// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"
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
    int Y() const                 { int result = (int)(xy >> 10); if (result & (1 << 13)) result |= 0xffffe000; return result; }
    int Height() const            { return Y(); }
    void Write(uint8 *dest) const { std::memcpy(dest, &xy, 3); }
    const uint *XY() const { return &xy; }
private:
    uint xy;
    uint8 *pointer;
};


struct SBuffer
{
    SBuffer(uint8 command) : pointer(0) { Push(command); }
    SBuffer(uint8 command, uint8 data) : pointer(0) { Push(command); Push(data); }
    SBuffer(uint8 command, const Point2 &p) : pointer(0) { Push(command); Push(p); }
    SBuffer(uint8 command, const Point2 &p1, const Point2 &p2) : pointer(0) { Push(command); Push(p1); Push(p2); }
    SBuffer(uint8 command, uint8 value, uint data) : pointer(0)
    {
        Push(command); Push(value); Push((uint8)data); Push((uint8)(data >> 8)); Push((uint8)(data >> 16)); Push((uint8)(data >> 24));
    }
    void Push(const Point2 &point)      { std::memcpy(&buffer[pointer], point.XY(), 3); pointer += 3; }
    void Push(uint8 byte)               { buffer[pointer++] = byte; }
    void Push(const uint8 *data, int num_bytes) { for (int i = 0; i < num_bytes; i++) { Push(*data++); } }

    void Send() const;

private:
    uint8 buffer[32];
    int pointer;
};


struct Command
{
    enum E
    {       //  номер байте            0  |     1       |     2      |      3      |     4        |      5       |   6    |   7   |  8   |  9  | Размер  |
            None,                   //                                                                                                         |         |
            ButtonPress,            // 01 |   Item      | TypePress  |                                                                         |    3    |
            Paint_BeginScene,       // 02 | num_field   |                                                                                      |    2    |
            Paint_EndScene,         // 03 |                                                                                                    |    1    |
            Paint_SetColor,         // 04 |   Color     |                                                                                      |    2    |
            Paint_FillRegion,       // 05 |              coord Point2              |             size Point2              |                    |    7    |
            Paint_DrawText,         // 06 |              coord Point2              | num symbols  |                                            |  5 + X  |
            Paint_SetPalette,       // 07 | numColor    | value[0:7] | value[8:15] | value[16:23] | value[24:31] |                             |    6    |
            Paint_DrawRectangle,    // 08 |              coord Point2              |             size Point2              |                    |    7    |
            Paint_DrawVLine,        // 09 |              x0 y0 Pont2               |              y1 Point2               |                    |    7    |
            Paint_DrawHLine,        // 10 |              x0 y0 Pont2               |              x1 Point2               |                    |    7    |
            Paint_SetFont,          // 11 | typeFont    |                                                                                      |    2    |
            Paint_SetPoint,         // 12 |              coord Point2              |                                                           |    4    |
            Paint_DrawLine,         // 13 |              coord0 Point2             |             coord1 Point2            |                    |    7    |
            Display_Brightness,     // 15 | яркость     |                                                                                      |    2    |
            Screen,                 // 16 |  numRow     |                                                                                      |    2    |
            Paint_VPointLine,       // 17 |              coord Point2              |   delta      |     count    |                             |    6    |
            Paint_HPointLine,       // 18 |              coord Point2              |   delta      |     count    |                             |    6    |
            Paint_SetMinWidthFont,  // 19 |   width     |                                                                                      |    2    |
            Paint_SetTextSpacing,   // 20 |   0 | 1     |                                                                                      |    2    |
            AddToConsole,           // 21 | num symb    |  ..... symbols ....                                                                  |  2 + X  |
            // mode/bits : 0 - 0/1 - точки/линии
            //             1 - 0/1 - пиквоый детектор откл/вкл
            Paint_DrawSignal,       // 23 |    mode     |          coord_left_top Point2          |     coord_left_bottom Point2  | num_points | 10 +  X |
            Null_command,           // 24 |   number
            //                                                                        в x хрантися длина линии, в skip - координата
            //                                                                места, которое нужно пропустить (там будет нарисован прямоугольник
            Paint_DrawVCursor,      // 25 |               coord                    |                height,skip           |                    |    7    |
            Paint_DrawHCursor,      // 26 |                                        |                                      |                    |    7    |
            Count
    };
};


// Извлекает режим рисования - линиями (1) или точками (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)
