// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Primitives.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstring>



Circle::Circle(int r) : radius(r)
{
}


void Circle::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < radius; i++)
    {
        float angle = std::asinf(i * (1.0F / radius));
        float length = std::cosf(angle) * radius;
        Draw2HLinesRelCenter(x + radius, y + i + radius - 1, (int)(length + 0.5F));
        Draw2HLinesRelCenter(x + radius, y - i + radius - 1, (int)(length + 0.5F));
    }
}


void Circle::Draw2HLinesRelCenter(int center, int y, int width)
{
    HLine(width).Draw(center, y);
    HLine(width).Draw(center - width, y);
}



Region::Region(int _width, int _height) : width(_width), height(_height)
{
}


void Region::DrawBounded(int x, int y, Color colorFill, Color colorBound)
{
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorFill);
    Rectangle(width, height).Draw(x, y, colorBound);
}


Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{
}
    

HLine::HLine(int _width) : width(_width)
{
}


VLine::VLine(int _height) : height(_height)
{
}


Line::Line(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1)
{
    if (x0 < 0)
    {
        x0 = 0;
    }
    if (y0 < 0)
    {
        y0 = 0;
    }
    if (x1 < 0)
    {
        x1 = 0;
    }
    if (y1 < 0)
    {
        y1 = 0;
    }
}


Char::Char(SymbolUGO::E _ch) : ch((char)(_ch)), font(TypeFont::_UGO)
{

}


Char::Char(SymbolUGO2::E _ch) : ch((char)(_ch)), font(TypeFont::_UGO2)
{

}

Char::Char(char _ch, TypeFont::E type) : ch(_ch), font(type)
{

}


int Char::Draw(int x, int y, Color color)
{
    Font::Set(font);

	String("%c", ch).Draw(x, y, color);

    int result = x + Symbol(ch).Width() + 1;

    Font::Pop();

    return result;
}


void Char::Draw4SymbolsInRect(int x, int y, Color color)
{
    color.SetAsCurrent();

    Font::Set(font);

    for (char i = 0; i < 2; i++)
    {
        String("%c", ch + i).Draw(x + 16 * i, y);
        String("%c", ch + i + 16).Draw(x + 16 * i, y + 16);
    }

    Font::Pop();
}


void Char::Draw10SymbolsInRect(int x, int y, Color color)
{
    color.SetAsCurrent();

    Font::Set(font);

    for (char i = 0; i < 5; i++)
    {
        String("%c", ch + i).Draw(x + 8 * i, y);
        String("%c", ch + i + 16).Draw(x + 8 * i, y + 8);
    }

    Font::Pop();
}


Text::Text(pchar _text) : text(_text)
{
}


Text::Text(const String &string)
{
    text = string.c_str();
}


int Text::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    if (text[0] != '\0')
    {
        x = DrawSmall(x, y);
    }

    return x;
}


int Text::DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int retValue = x;

    while (*text)
    {
        x = DrawCharWithLimitation(x, y, *text, limitX, limitY, limitWidth, limitHeight);
        retValue += Symbol(*text).Width();
        text++;
    }

    return retValue + 1;
}


int Text::DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    Symbol symbol(_symbol);

    int width = symbol.Width();
    int8 height = (int8)Font::GetHeight();

    for (int b = 0; b < height; b++)
    {
        if(Font::RowNotEmpty((uint8)_symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist((uint8)_symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Pixel().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}


int Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeight();
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2 + 1;
    return Draw(x, y, color);
}


int Text::DrawDigitsMonospace(int x, int y, int width, Color color)
{
    color.SetAsCurrent();

    uint size = std::strlen(text);

    for (uint i = 0; i < size; i++)
    {
        Symbol symbol(text[i]);

        int dX = 0;

        if (symbol >= 0x30 && symbol <= 0x39)
        {
            int widthSymbol = symbol.Width();

            dX = (width - widthSymbol) / 2;

            x += dX;
        }

        x = Text(String("%c", symbol.symbol)).Draw(x, y);
        x += Font::GetSpacing();

        if (symbol >= 0x30 && symbol <= 0x39)
        {
            x += dX;
        }
    }

    return x;
}


void Text::DrawInCenterBoundedRect(int x, int y, int width, int height, Color color)
{
    Region(width, height).DrawBounded(x, y, Color::BACK, color);
    DrawInCenterRect(x, y, width, height, color);
}


int Text::DrawOnBackground(int x, int y, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetHeight();

    Color colorText(Color::GetCurent());
    Region(width, height).Fill(x - 1, y, colorBackground);

    return Draw(x, y, colorText);
}


void Text::DrawRelativelyRight(int xRight, int y, Color color)
{
    Draw(xRight - Font::GetLengthText(text), y, color);
}


static bool IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[(uint8)(symbol)];
}


static char *GetWord(pchar firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}


static bool IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)(symbol)];
}


static bool CompareArrays(const bool *array1, const bool *array2, int numElems)
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}


// \brief ������� ��������� �������. C letters ���������� ����� �����, ��� ����� ����� �������, � lettersInSyllable ����� �������� ����� ���� � 
// ��������� �����. ���� ����� �����������, ������� ���������� false
static bool FindNextTransfer(pchar letters, int8 *lettersInSyllable)
{

#define VOWEL       0   // �������
#define CONSONANT   1   // ���������

    *lettersInSyllable = (int8)std::strlen(letters);

    if (std::strlen(letters) <= 3) //-V1051
    {
        return false;
    }

    static const bool template1[3] = { false, true, true };               //     011     2   // ����� ������� ������� �������
    static const bool template2[4] = { true, false, true, false };        //     1010    2
    static const bool template3[4] = { false, true, false, true };        //     0101    3
    static const bool template4[4] = { true, false, true, true };         //     1011    3
    static const bool template5[4] = { false, true, false, false };       //     0100    3
    static const bool template6[4] = { true, false, true, true };         //     1011    3
    static const bool template7[5] = { true, true, false, true, false };  //     11010   3
    static const bool template8[6] = { true, true, false, true, true };   //     11011   4

    bool consonant[20];

    uint size = std::strlen(letters);

    for (uint i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (std::strlen(letters) < 5)
    {
        return false;
    }

    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) ||
        CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (std::strlen(letters) < 6)
    {
        return false;
    }

    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }

    return false;
}


static int8 *BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;

    if (std::strcmp(word, "���������") == 0)
    {
        int8 lengths[] = { 5, 2, 2, 0 };
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "���������������") == 0)
    {
        int8 lenghts[] = { 4, 3, 4, 5, 3, 0 };
        std::memcpy(lengthSyllables, lenghts, 6);
    }

    return lengthSyllables;
}


// ���������� ����� ����� �� ����� numSyllable(�����������) ������ �� ������ ��������
static char *PartWordForTransfer(char *word, const int8 *lengthSyllables, int numSyllable, char buffer[30])
{
    uint length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += (uint)(lengthSyllables[i]);
    }
    std::memcpy((void *)(buffer), (void *)(word), length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


// ���� draw == false, �� �������� ������ �� ����, ������ ������������ ������ ��� ����������
static int DrawPartWord(char *word, int x, int y, int xRight, bool draw)
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                String(subString).Draw(x, y);
            }
            return (int)(std::strlen(subString) - 1);
        }
    }

    return 0;
}


int Text::DrawInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, Color color)
{
    color.SetAsCurrent();

    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    char buffer[20];
    int numSymbols = (int)(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x = Char(symbol).Draw(x, y);
            }
            else                                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = Text(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


// ���������� ������ ������, ������� ����� ����� text, ��� ������ �� left �� right � ���������� height. ���� bool == false, �� ����� �� ������ �� ����� 
static bool GetHeightTextWithTransfers(int left, int top, int right, pchar text, int *height)
{
    char buffer[20];
    int numSymbols = (int)(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                Symbol symbol(text[curSymbol++]);

                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }

                x += symbol.Width();
            }
            else                                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    *height = y - top + 4;

    LIMITATION(*height, 0, 239);

    return curSymbol == numSymbols;
}


int Text::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill, Color colorBound)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Rectangle(width, height).Draw(x, y, (colorBound == Color::Count) ? colorFill : colorBound);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    DrawInRectWithTransfers(x + 3, y + 3, width - 8, height, colorFill);
    return y + height;
}


void Text::DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill)
{
    Region(width, height).DrawBounded(x, y, colorBackground, colorFill);

    colorFill.SetAsCurrent();

    DrawInCenterRect(x, y, width, height);
}


void Text::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Fill(eX - lenght - widthBorder, y - widthBorder + 1);
    DrawInCenterRect(x, y, width, height, colorText);
}


DashedHLine::DashedHLine(int _width, int _deltaFill, int _deltaEmpty, int _deltaStart) : 
    width(_width), deltaFill(_deltaFill), deltaEmpty(_deltaEmpty), deltaStart(_deltaStart)
{
}


void DashedHLine::Draw(int x0, int y)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        //LOG_ERROR("������������ �������� deltaStart = %d", deltaStart);
        return;
    }

    int x = x0;

    if (deltaStart != 0)                // ���� ����� ����� �������� �� � ������ ������
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            HLine(x - 1 - x0).Draw(x0, y);
        }
    }

    int x1 = x0 + width;

    while (x < x1)
    {
        HLine(deltaFill - 1).Draw(x, y);

        x += (deltaFill + deltaEmpty);
    }
}


VPointLine::VPointLine(int _height, float _delta) : height(_height), delta(_delta)
{
}


void VPointLine::Draw(int _x, int _y, Color color)
{
    color.SetAsCurrent();

    int y0 = _y;
    int y1 = y0 + height;

    for (int y = y0; y <= y1; y += (int)(delta))
    {
        Pixel().Draw(_x, y);
    }
}


HPointLine::HPointLine(int _width, float _delta) : width(_width), delta(_delta)
{
}


void HPointLine::Draw(int _x, int _y)
{
    int x0 = _x;
    int x1 = _x + width;

    for (int x = x0; x <= x1; x += (int)(delta))
    {
        Pixel().Draw(x, _y);
    }
}


MultiHPointLine::MultiHPointLine(int _numLines, const uint16 *_y, int _delta, int _count) :
    numLines(_numLines), y(_y), delta(_delta), count(_count)
{
}


VLineArray::VLineArray(int _numLines, uint8 *_y0y1) : numLines(_numLines), y0y1(_y0y1)
{
}


void VLineArray::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        uint8 y0 = *y0y1++;
        uint8 y1 = *y0y1++;
        VLine(y1 - y0).Draw(x, y0);
    }
}


MultiVPointLine::MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count)
{
}
