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

    static Color GRAY_10;
    static Color GRAY_20;
    static Color GRAY_50;
    static Color GRAY_75;

    static Color DATA_HALF_A;
    static Color DATA_HALF_B;
    static Color SEPARATOR;
    static Color YELLOW;
    static Color BATTERY;
    static Color BATTERY_EMPTY;     // ���� ���� ������������ ��� ��������� ������� ���� ��� ��������� ������ �������


    static Color NUMBER;

    static Color FILL;
    static Color BACK;
    static Color FLASH_10;
    static Color FLASH_01;

    explicit Color(uint8 val) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    static Color Trig();
    // ������������� ������� ���� ���������
    void SetAsCurrent();
    // ���������� ������� ���� ���������
    static Color GetCurent();

    static void Log(Color color);

    void LoadValueRGB();

    Color& operator=(const Color &color);

    uint8 value;

    static Color CHAN[4];
    // ����������� "��������" ����. ���� reset == true, ����������� �� �����
    static void ChangeFlash(bool reset = false);

private:
   
    enum
    {
        COLOR_BLACK = 0,
        COLOR_WHITE = 1,
        COLOR_GRID = 2,
        COLOR_DATA_A = 3,
        COLOR_DATA_B = 4,
        COLOR_MENU_FIELD = 5,
        COLOR_MENU_TITLE = 6,
        COLOR_MENU_TITLE_DARK = 7,
        COLOR_MENU_TITLE_BRIGHT = 8,
        COLOR_BATTERY_EMPTY = 9,
        COLOR_MENU_ITEM_DARK = 10,
        COLOR_MENU_ITEM_BRIGHT = 11,
        COLOR_DATA_WHITE_ACCUM_A = 12,   // ������������ ��� ��� ��������� ������ �� ����� ����, ��� � ��� ��������� ����������� ��������
        COLOR_DATA_WHITE_ACCUM_B = 13,
        COLOR_RED = 14,
        COLOR_GREEN = 15,
        COLOR_BLUE = 16,


        COLOR_GRAY_10 = 19,
        COLOR_GRAY_20 = 20,
        COLOR_BLUE_10 = 21,
        COLOR_GRAY_50 = 22,
        COLOR_GRAY_75 = 23,
        COLOR_DATA_HALF_A = 24,
        COLOR_DATA_HALF_B = 25,
        COLOR_SEPARATOR = 26,
        COLOR_YELLOW = 27,
        COLOR_BATTERY = 28,
        

        COLOR_NUMBER = 32,

        COLOR_FLASH_10 = 33,
        COLOR_FLASH_01 = 34,
        COLOR_INVERSE = 35
    };

    static Color currentColor;
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);


// ��������� ������������ ��� ���������� ��������� �����
class ColorType
{
public:
    // ������� ������������ ����� [0.0...1.0]
    float   red;
    // ������ ������������ ����� [0.0...1.0]
    float   green;
    // ����� ������������ ����� [0.0...1.0]
    float   blue;
    // ��� ��������� ������� ������������ �����
    float   stepRed;
    // ��� ��������� ������ ������������ �����
    float   stepGreen;
    // ��� ��������� ����� ������������ �����
    float   stepBlue;
    // ������� [0...1]
    float   brightness;
    Color   color;
    int8    currentField;
    // true, ���� ��������� ��� ����������������
    bool    inititalized;

    // �������������. ��� ��������� ������ �� ��������. ��� ��������� ������� �������� ReInit()
    void Init();
    void ReInit();
    void SetBrightness(float brightness = -1.0F);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
    void ApplyColor();
private:
    void CalcSteps();
    
};

#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false};


#define MAKE_COLOR(r, g, b) (static_cast<uint>((b) + ((g) << 8) + ((r) << 16)))
#define R_FROM_COLOR(color) ((static_cast<uint>(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) ((static_cast<uint>(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) ((static_cast<uint>(color))       & 0xff)

#define COLOR(x) GlobalColors[x]

extern uint GlobalColors[256];
