// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"
#include "common/Display/Font/Symbols.h"
#include "common/Display/Font/Font.h"



class Circle
{
public:
    Circle(int radius);
    void Fill(int x, int y, Color color = Color::Count);
private:
    int radius;
    // Нарисовать линии вправо и влево отностиельно центра с длиной width
    static void Draw2HLinesRelCenter(int center, int y, int width);
};



class Region
{
public:
    Region(int width, int height);
    void Fill(int x, int y, Color color = Color::Count);
    void DrawBounded(int x, int y, Color colorFill, Color colorBound);
private:
    int width;
    int height;
};



class Rectangle
{
public:
    Rectangle(int width, int height);
    void Draw(int x, int y, Color color = Color::Count);
private:
    int width;
    int height;
};



class HLine
{
public:
    HLine(int width);
    void Draw(int x, int y, Color color = Color::Count);
private:
    int width;
};



class VLine
{
public:
    VLine(int height);
    void Draw(int x, int y, Color color = Color::Count);
private:
    int height;
};



class Line
{
public:
    Line(int x0, int y0, int x1, int y1);
    void Draw(Color color = Color::Count);
private:
    int x0;
    int y0;
    int x1;
    int y1;
};


class Char
{
public:
    explicit Char(char ch, TypeFont::E font = TypeFont::Normal, int num = 1);
    int Draw(int x, int y, Color color = Color::Count);
    void Draw4SymbolsInRect(int x, int y, Color color = Color::Count);
protected:
    const char ch;
    const TypeFont::E font;
    const int num_symbols;        // Может быть 1, 2, 4 или 10
private:
    void Draw2SymbolsInRect(int x, int y, Color color = Color::Count);
    void Draw10SymbolsInRect(int x, int y, Color color = Color::Count);
};


class SymbolUGO : public Char
{
public:
    enum E
    {
        RSHIFT_NORMAL,  // Символ смещения по напряжению нормальный
        Count
    };

    SymbolUGO(E);

private:

    struct SymbolStruct
    {
        char code;
        int num_symbols;
    };

    static const SymbolStruct desc[Count];
};


class Pixel
{
public:
    Pixel() {};
    void Draw(int x, int y, Color color = Color::Count);
private:
};


class Text
{
public:

    Text(pchar text);
    Text(const String &string);

    pchar c_str() { return text; }

    int Draw(int x, int y, Color color = Color::Count);

    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::Count, bool log = false);

    void DrawInCenterBoundedRect(int x, int y, int width, int height, Color color = Color::Count);

    int DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight);

    // Выводит текст на прямоугольнике цвета colorBackgound
    int DrawOnBackground(int x, int y, Color colorBackground);

    void DrawRelativelyRight(int xRight, int y, Color color = Color::Count);

    // Пишет текст с переносами
    int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::Count);

    // Возвращает нижнюю координату прямоугольника
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill, Color colorBound = Color::Count);

    void DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);

    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);

    // Нарисовать с выводом цифр в моноширинных позициях
    int DrawDigitsMonospace(int x, int y, int width, Color color = Color::Count);

private:
    pchar text;
    int DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    int DrawSmall(int x, int y, Color color = Color::Count);
};


class VCursor
{
public:
    VCursor(int _height, int _skip) : height(_height), skip(_skip) { }
    void Draw(int x, int y);
private:
    int height;
    int skip;
};


class DashedHLine
{
public:
    DashedHLine(int width, int deltaFill, int deltaEmpty, int deltaStart);
    void Draw(int x, int y);
private:
    int width;
    int deltaFill;
    int deltaEmpty;
    int deltaStart;
};


// Вертикальная линия от y до y + height точками через каждые delta пикселей
class VPointLine
{
public:
    VPointLine(int height, float delta);
    void Draw(int x, int y, Color color = Color::Count);
private:
    int height;
    float delta;
};


// Горизонтальная линия от x до x + width точками через каждые delta пикселей
class HPointLine
{
public:
    HPointLine(int width, float delta);
    void Draw(int x, int y);
private:
    int width;
    float delta;
};


// \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
// первой точки каждой линии соответствует очередному элементу массива y[]
class MultiHPointLine
{
public:
    MultiHPointLine(int numLines, const uint16 *y, int delta, int count);
    void Draw(int x, Color color = Color::Count);
private:
    int numLines;
    const uint16 *y;
    int delta;
    int count;
};


// Массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
class VLineArray
{
public:
    VLineArray(int numLines, uint8 *y0y1);
    void Draw(int x, Color color);
private:
    int numLines;
    uint8 *y0y1;
};


// numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
// первой точки каждой линии соответствует очередному элементу массива x[]
class MultiVPointLine
{
public:
    MultiVPointLine(int numLines, uint16 *x0, int delta, int count);
    void Draw(int y0, Color color = Color::Count);
private:
    int numLines;
    uint16 *x0;
    int delta;
    int count;
};
