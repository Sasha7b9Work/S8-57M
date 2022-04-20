// 2022/04/20 16:51:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Command
{           //  смещение               0  |     1       |     2      |      3      |     4        |      5       |   6    |         –азмер
    enum E                                              
    {                                                   
/* 00 */    None,                   //    |             |            |             |              |              |        |
/* 01 */    ButtonPress,            // 01 |   Item      | TypePress  |             |              |              |        |
/* 02 */    Display_Brightness,     // 15 | €ркость
/* 03 */    Screen,                 // 16 |  numRow     |
/* 04 */    AddToConsole,           // 21 | num symb    |  ..... symbols ....

/* 05 */    Paint_DirectLine,       // ќтрисовка "пр€мой" линии
            //  код команды [8] | число байт в команде (начина€ со следующего) [8] | номер цвета [8] | количество точек [8]

/* 06 */    Paint_DifferentLine,    // ќтрисовка "разностной" линии
            //  код команды [8] | число баайт в команде (начина€ со следующего) [8] | разность цвета по сравнению с прошлым кадром [8] | количество точек [8]

            Count
    };
};


#define TESTER_NUM_POINTS 120


#define SIZE_STRING (320)


// »звлекает режим рисовани€ - лини€ми (1) или точками (0)
#define BUILD_MODE(modeDraw, step, enumAverage) ((uint8)(((enumAverage) << 4) + ((step) << 1) + (modeDraw)))

#define EXTRACT_MODE_DRAW(x)    ((x) & 0x01)
#define EXTRACT_STEP(x)         (((x) >> 1) & 0x07)
#define EXTRACT_ENUM_AVERAGE(x) (((x) >> 4) & 0x0f)
