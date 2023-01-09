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
    static Color DATA_WHITE_ACCUM_A;
    static Color DATA_WHITE_ACCUM_B;
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
    static Color DATA_HALF_A;
    static Color DATA_HALF_B;
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

    static void Log(Color color);

    void LoadValueRGB();

    Color& operator=(const Color &color);

    uint8 value;

    static Color CHAN[4];
    // Переключить "мигающий" цвет. Если reset == true, сбрасывется на белый
    static void ChangeFlash(bool reset = false);

private:
   
    enum
    {
        _BLACK = 0,
        _WHITE = 1,
        _GRID = 2,
        _DATA_A = 3,
        _DATA_B = 4,
        _MENU_FIELD = 5,
        _MENU_TITLE = 6,
        _MENU_TITLE_DARK = 7,
        _MENU_TITLE_BRIGHT = 8,

        _MENU_ITEM_DARK = 10,
        _MENU_ITEM_BRIGHT = 11,
        _DATA_WHITE_ACCUM_A = 12,   ///< Используется как для отрисовки канала на белом фоне, так и для отрисовки накопленных сигналов
        _DATA_WHITE_ACCUM_B = 13,
        _RED = 14,
        _GREEN = 15,
        _BLUE = 16,
        _BLUE_25 = 17,
        _BLUE_50 = 18,
        _GRAY_10 = 19,
        _GRAY_20 = 20,
        _BLUE_10 = 21,
        _GRAY_50 = 22,
        _GRAY_75 = 23,
        _DATA_HALF_A = 24,
        _DATA_HALF_B = 25,
        _SEPARATOR = 26,
        _YELLOW = 27,
        _BATTERY = 28,

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
