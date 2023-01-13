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
    // ���������� ����� ������ � ����� ������������ ������ � ������ width
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
    const int num_symbols;        // ����� ���� 1, 2, 4 ��� 10
private:
    void Draw2SymbolsInRect(int x, int y, Color color = Color::Count);
    void Draw10SymbolsInRect(int x, int y, Color color = Color::Count);
};


class SymbolUGO : public Char
{
public:
    enum E
    {
        RSHIFT_NORMAL,  // ������ �������� �� ���������� ����������
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

    // ������� ����� �� �������������� ����� colorBackgound
    int DrawOnBackground(int x, int y, Color colorBackground);

    void DrawRelativelyRight(int xRight, int y, Color color = Color::Count);

    // ����� ����� � ����������
    int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::Count);

    // ���������� ������ ���������� ��������������
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill, Color colorBound = Color::Count);

    void DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);

    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);

    // ���������� � ������� ���� � ������������ ��������
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


// ������������ ����� �� y �� y + height ������� ����� ������ delta ��������
class VPointLine
{
public:
    VPointLine(int height, float delta);
    void Draw(int x, int y, Color color = Color::Count);
private:
    int height;
    float delta;
};


// �������������� ����� �� x �� x + width ������� ����� ������ delta ��������
class HPointLine
{
public:
    HPointLine(int width, float delta);
    void Draw(int x, int y);
private:
    int width;
    float delta;
};


// \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. ������������ ����������
// ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
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


// ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
class VLineArray
{
public:
    VLineArray(int numLines, uint8 *y0y1);
    void Draw(int x, Color color);
private:
    int numLines;
    uint8 *y0y1;
};


// numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
// ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
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
