#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"


extern uint GlobalColors[256] =
{
    /* 0  */    MAKE_COLOR(0x00, 0x00, 0x00),       // BLACK
    /* 1  */    MAKE_COLOR(0xff, 0xff, 0xff),       // WHITE
    /* 2  */    MAKE_COLOR(0x60, 0x60, 0x60),       // GRID
    /* 3  */    MAKE_COLOR(0x00, 0xdf, 0xff),       // DATA_A
    /* 4  */    MAKE_COLOR(0x00, 0xff, 0x00),       // DATA_B
    /* 5  */    MAKE_COLOR(0x90, 0x90, 0xa0),       // MENU_FIELD
    /* 6  */    MAKE_COLOR(31 * 8, 44 * 4, 0  * 8), // MENU_TITLE
    /* 7  */    MAKE_COLOR(0x00, 0x00, 0x80),       // MENU_TITLE_DARK
    /* 8  */    MAKE_COLOR(31 * 8, 63 * 4, 0  * 8), // MENU_TITLE_BRIGHT
    /* 9  */    MAKE_COLOR(26 * 8, 34 * 4, 0  * 8), // 
    /* 10 */    MAKE_COLOR(13 * 8, 17 * 4, 0  * 8), // MENU_ITEM_DARK
    /* 11 */    MAKE_COLOR(31 * 8, 51 * 4, 0  * 8), // MENU_ITEM_BRIGHT
    /* 14 */    MAKE_COLOR(0xff, 0x00, 0x00),       // RED
    /* 15 */    MAKE_COLOR(0x00, 0xff, 0x00),       // GREEN
    /* 16 */    MAKE_COLOR(0x00, 0x00, 0xff),       // BLUE
    /* 17 */    MAKE_COLOR(0x00, 0x00, 0x40),       // BLUE_25
    /* 18 */    MAKE_COLOR(0x00, 0x00, 0x80),       // BLUE_50
    /* 19 */    MAKE_COLOR(0x1a, 0x1a, 0x1a),       // GRAY_10
    /* 20 */    MAKE_COLOR(0x33, 0x33, 0x33),       // GRAY_20
    /* 21 */    MAKE_COLOR(0x00, 0x00, 0x1a),       // BLUE_10
    /* 22 */    MAKE_COLOR(0x80, 0x80, 0x80),       // GRAY_50
    /* 23 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // GRAY_75
    /* 24 */    MAKE_COLOR(0x00, 0x6f, 0x80),       // DATA_HALF_A
    /* 25 */    MAKE_COLOR(0x00, 0x80, 0x00),       // DATA_HALF_B
    /* 26 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // SEPARATOR
    /* 27 */    MAKE_COLOR(0xff, 0xff, 0x00),       // YELLOW
    /* 28 */    MAKE_COLOR(0x00, 0x7f, 0x00)        // BATTERY
};


Color Color::BLACK(_BLACK);
Color Color::WHITE(_WHITE);
Color Color::MENU_FIELD(_MENU_FIELD);
Color Color::MENU_TITLE_DARK(_MENU_TITLE_DARK);
Color Color::MENU_TITLE_BRIGHT(_MENU_ITEM_BRIGHT);
Color Color::MENU_ITEM_DARK(_MENU_ITEM_DARK);
Color Color::MENU_ITEM_BRIGHT(_MENU_ITEM_BRIGHT);
Color Color::RED(_RED);
Color Color::GREEN(_GREEN);
Color Color::BLUE(_BLUE);
Color Color::BLUE_25(_BLUE_25);
Color Color::BLUE_50(_BLUE_50);
Color Color::GRAY_10(_GRAY_10);
Color Color::GRAY_20(_GRAY_20);
Color Color::BLUE_10(_BLUE_10);
Color Color::GRAY_50(_GRAY_50);
Color Color::GRAY_75(_GRAY_75);
Color Color::SEPARATOR(_SEPARATOR);
Color Color::YELLOW(_YELLOW);
Color Color::BATTERY(_BATTERY);

Color Color::Count(_NUMBER);
Color Color::FLASH_10(_FLASH_10);
Color Color::FLASH_01(_FLASH_01);

Color Color::CHAN[4] = {Color(_DATA_A), Color(_DATA_B), Color(_WHITE), Color(_WHITE)};
Color Color::FILL(_WHITE);
Color Color::BACK(_BLACK);
Color Color::GRID(_GRID);


Color Color::currentColor = Color::Count;
static bool  inverseColor = false;


/// ���������� ������� ���� � �������. ���������� false, ���� ������� ���� ����������
static bool WriteFlashColor();
/// ���������� ���� � �������
static void WriteToDisplay(Color color);


void Color::Log(Color)
{
#define colorVal (COLOR(color.value))

    //LOG_WRITE("Color %d R=%d, G=%d, B=%d", color.value, R_FROM_COLOR(colorVal), G_FROM_COLOR(colorVal), B_FROM_COLOR(colorVal));
}


bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}


bool operator==(const Color &left, const Color &right)
{
    return left.value == right.value;
}


bool operator>(const Color &left, const Color &right)
{
    return left.value > right.value;
}


/*
�������� ��������� �������.
1. �������������.
�. ���������� ������� �� �������� - ������� ����� ������������� ������������� ������ ������ ��������� ������
�. ���������� ��� ��������� �������� ������������ ������� ������ �� 1% ������� �� �������:
��� = ������������� ������ * �������, ��� ������� - ������������� �������� ����� �������
�. ���� ������������� ���� ������� == 0, �� ���� ������ ��� ������� ������
2. ��� ���������� ������� �� 1% ������ ������������� ������� ������ �� ���, ����������� � ���������� ������.
3. ��� ��������� ������������� ��������� ������ ������������� ������� � ��� ��������� ������� ������.
*/


Color& Color::operator=(const Color &color)
{
    value = color.value;
    return *this;
}


Color Color::GetCurent()
{
    return currentColor;
}


static bool WriteFlashColor()
{
    bool result = false;

    if (Color::GetCurent() == Color::FLASH_01)
    {
        WriteToDisplay(inverseColor ? Color::FILL : Color::BACK);
        result = true;
    }
    if (Color::GetCurent() == Color::FLASH_10)
    {
        WriteToDisplay(inverseColor ? Color::BACK : Color::FILL);
        result = true;
    }

    return result;
}


static void WriteToDisplay(Color color)
{
    static Color lastColor = Color::Count;

    if (color != lastColor)
    {
        lastColor = color;

        SBuffer(Command::Paint_SetColor, lastColor.value).Send();
    }
}


void Color::LoadValueRGB()
{
    SBuffer(Command::Paint_SetPalette, value, COLOR(value)).Send();
}


void Color::SetAsCurrent()
{
    if(value != Color::Count.value)
    {
        currentColor = *this;
    }

    if(!WriteFlashColor())
    {
        WriteToDisplay(currentColor);
    }
}


void Color::ChangeFlash(bool reset)
{
    static uint time = 0;

    if (reset)
    {
        inverseColor = true;
        time = 0;
    }

    if (TIME_MS - time < 500)
    {
        return;
    }

    time = TIME_MS;

    inverseColor = !inverseColor;
    WriteFlashColor();
}
