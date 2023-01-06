#pragma once
#include "Utils/String.h"
#include "common/Display/Font/Font_d.h"


class Region
{
public:
    Region(int width, int height);
    void Fill(int x, int y, Color color = Color::NUMBER);
    void DrawBounded(int x, int y, Color colorFill, Color colorBound);
private:
    int width;
    int height;
};


class Rectangle
{
public:
    Rectangle(int width, int height);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int width;
    int height;
};



class HLine
{
public:
    HLine(int width);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int width;
};



class VLine
{
public:
    VLine(int height);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int height;
};



class Line
{
public:
    Line(int x0, int y0, int x1, int y1);
    void Draw(Color color = Color::NUMBER);
private:
    int x0;
    int y0;
    int x1;
    int y1;
};



class Char
{
public:
    explicit Char(char ch, DTypeFont::E font = DTypeFont::_8);
    int Draw(int x, int y, Color color = Color::NUMBER);
    void Draw4SymbolsInRect(int x, int y, Color color = Color::NUMBER);
    void Draw10SymbolsInRect(int x, int y, Color color = Color::NUMBER);
private:
    char ch;
    DTypeFont::E font;
};



class Pixel
{
public:
    Pixel() {};
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
};



class Text
{
public:
    Text(pchar text);
    Text(const String &string);

    int Draw(int x, int y, Color color = Color::NUMBER);

    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawInCenterBoundedRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    int DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight);
    /// ������� ����� �� �������������� ����� colorBackgound
    int DrawOnBackground(int x, int y, Color colorBackground);

    void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
    /// ����� ����� � ����������
    int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::NUMBER);
    /// ���������� ������ ���������� ��������������
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill, Color colorBound = Color::NUMBER);

    void DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);
    /// ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);
    /// ���������� � ������� ���� � ������������ ��������
    int DrawDigitsMonospace(int x, int y, int width, Color color = Color::NUMBER);

private:
    pchar text;
    int DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    int DrawSmall(int x, int y, Color color = Color::NUMBER);
};


class DashedVLine
{
public:
    DashedVLine(int height, int deltaFill, int deltaEmpty, int deltaStart);
    void Draw(int x, int y);
private:
    int height;
    int deltaFill;
    int deltaEmpty;
    int deltaStart;
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


/// ������������ ����� �� y �� y + height ������� ����� ������ delta ��������
class VPointLine
{
public:
    VPointLine(int height, float delta);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int height;
    float delta;
};


/// �������������� ����� �� x �� x + width ������� ����� ������ delta ��������
class HPointLine
{
public:
    HPointLine(int width, float delta);
    void Draw(int x, int y);
private:
    int width;
    float delta;
};


/// ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
class VLineArray
{
public:
    VLineArray(int numLines, uint8 *y0y1);
    void Draw(int x, Color color);
private:
    int numLines;
    uint8 *y0y1;
};
