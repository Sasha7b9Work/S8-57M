#pragma once


class Color
{
private:
    static Color MENU_FIELD;

public:
    static Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM_DARK;
    static Color MENU_ITEM_BRIGHT;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color BLUE_25;
    static Color BLUE_50;
    static Color GRAY_10;
    static Color GRAY_20;
    static Color BLUE_10;
    static Color GRAY_50;
    static Color GRAY_75;
    static Color SEPARATOR;
    static Color YELLOW;
    static Color BATTERY;

    static Color Count;

    static Color FILL;
    static Color BACK;
    static Color FLASH_10;
    static Color FLASH_01;

    explicit Color(uint8 val) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    // Устанавливает текущий цвет отрисовки
    void SetAsCurrent();
    // Возвращает текущий цвет отрисовки
    static Color GetCurent();

    static void Log(Color);

    void LoadValueRGB();

    Color& operator=(const Color &);

    uint8 value;

    static Color CHAN[4];
    // Переключить "мигающий" цвет. Если reset == true, сбрасывется на белый
    static void ChangeFlash(bool reset = false);

private:
   
    enum
    {
        _BLACK,
        _WHITE,
        _GRID,
        _DATA_A,
        _DATA_B,
        _MENU_FIELD,
        _MENU_TITLE,
        _MENU_TITLE_DARK,
        _MENU_TITLE_BRIGHT,
        _MENU_ITEM_DARK,
        _MENU_ITEM_BRIGHT,
        _RED,
        _GREEN,
        _BLUE,
        _BLUE_25,
        _BLUE_50,
        _GRAY_10,
        _GRAY_20,
        _BLUE_10,
        _GRAY_50,
        _GRAY_75,
        _SEPARATOR,
        _YELLOW,
        _BATTERY,

        _NUMBER = 64,

        _FLASH_10,
        _FLASH_01,
        _INVERSE
    };

    static Color currentColor;
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);


#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false};


#define MAKE_COLOR(r, g, b) ((uint)((b) + ((g) << 8) + ((r) << 16)))
#define R_FROM_COLOR(color) (((uint)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((uint)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((uint)(color))       & 0xff)

#define COLOR(x) GlobalColors[x]

extern uint GlobalColors[256];
