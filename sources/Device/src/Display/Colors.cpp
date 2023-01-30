// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "common/Command.h"


uint Color::raw[256] =
{
    MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
    MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
    MAKE_COLOR(0x60, 0x60, 0x60),   // GRID
    MAKE_COLOR(0x00, 0xdf, 0xff),   // DATA_A
    MAKE_COLOR(0x00, 0xff, 0x00),   // DATA_B
    MAKE_COLOR(0x90, 0x90, 0xa0),   // MENU_FIELD
    MAKE_COLOR(0xf8, 0xb0, 0x00),   // MENU_TITLE
    MAKE_COLOR(0x00, 0x00, 0x80),   // MENU_TITLE_DARK
    MAKE_COLOR(0xf8, 0xfc, 0x00),   // MENU_TITLE_BRIGHT
    MAKE_COLOR(0x68, 0x44, 0x00),   // MENU_ITEM_DARK
    MAKE_COLOR(0xf8, 0xcc, 0x00),   // MENU_ITEM_BRIGHT
    MAKE_COLOR(0xff, 0x00, 0x00),   // RED
    MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
    MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
    MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
    MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
    MAKE_COLOR(0x1a, 0x1a, 0x1a),   // GRAY_10
    MAKE_COLOR(0x33, 0x33, 0x33),   // GRAY_20
    MAKE_COLOR(0x00, 0x00, 0x1a),   // BLUE_10
    MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
    MAKE_COLOR(0xc0, 0xc0, 0xc0),   // GRAY_75
    MAKE_COLOR(0xc0, 0xc0, 0xc0),   // SEPARATOR
    MAKE_COLOR(0xff, 0xff, 0x00),   // YELLOW
    MAKE_COLOR(0x00, 0x7f, 0x00)    // BATTERY
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
Color Color::GRAY_10(_GRAY_10);
Color Color::GRAY_20(_GRAY_20);
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


// Записывает мигающй цвет в дисплей. Возвращает false, если текущий цвет немигающий
static bool WriteFlashColor();
// Записывает цвет в дисплей
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


void ColorType::Init()
{
    if (!inititalized)
    {
        ReInit();
        inititalized = true;
    }
}


void ColorType::ReInit()
{
    uint colorValue = COLOR(color.value);

    red = (float)R_FROM_COLOR(colorValue);
    green = (float)G_FROM_COLOR(colorValue);
    blue = (float)B_FROM_COLOR(colorValue);

    SetBrightness();

    if (Math::IsEquals(red, 0.0F) &&
        Math::IsEquals(green, 0.0F) &&
        Math::IsEquals(blue, 0.0F))
    {
        stepRed = 0.31F;
        stepGreen = 0.63F;
        stepBlue = 0.31F;
    }
}


void ColorType::SetBrightness(float bright)
{
    if (Math::IsEquals(bright, -1.0F))
    {
        brightness = Math::Max(red / 255.0F, green / 255.0F, blue / 255.0F);

        CalcSteps();
    }
    else
    {
        int delta = (int)((bright + 0.0005F) * 100.0F) - (int)(brightness * 100.0F);

        if (delta > 0)
        {
            for (int i = 0; i < delta; i++)
            {
                BrightnessChange(1);
            }
        }
        else
        {
            for (int i = 0; i < -delta; i++)
            {
                BrightnessChange(-1);
            }
        }
    }
}


/*
Алгоритм изменения яркости.
1. Инициализация.
а. Рассчитать яркость по принципу - яркость равна относительной интенсивности самого яркого цветового канала
б. Рассчитать шаг изменения цветовой составляющей каждого канала на 1% яркости по формуле:
Шаг = Интенсивность канала * Яркость, где Яркость - относительная величина общей яркости
в. Если интенсивность всех каналов == 0, то дать полный шаг каждому каналу
2. При изменнении яркости на 1% менять интенсивность каждого канала на Шаг, расчитанный в предыдущем пункте.
3. При изменения интенсивности цветового канала пересчитывать яркость и шаг изменения каждого канала.
*/


void ColorType::BrightnessChange(int delta) // -V2506
{
    if ((delta > 0 && brightness == 1.0F) || (delta < 0 && brightness == 0.0F)) // -V550
    {
        return;
    }

    float sign = (float)Math::Sign(delta);

    brightness += sign * 0.01F;
    LIMITATION(brightness, 0.0F, 1.0F);

    red += sign * stepRed;
    green += sign * stepGreen;
    blue += sign * stepBlue;

    ApplyColor();

    if (stepRed < 0.01F && stepGreen < 0.01F && stepBlue < 0.01F)
    {
        stepRed = 2.55F;
        stepGreen = 2.55F;
        stepBlue = 2.55F;
    }
}


void ColorType::CalcSteps()
{
    stepRed = red / (brightness * 100.0F);
    stepGreen = green / (brightness * 100.0F);
    stepBlue = blue / (brightness * 100.0F);
}


void ColorType::ApplyColor()
{
    LIMITATION(red, 0.0F, 255.0F);
    LIMITATION(green, 0.0F, 255.0F);
    LIMITATION(blue, 0.0F, 255.0F);

    COLOR(color.value) = MAKE_COLOR((int)(red), (int)(green), (int)(blue));
    color.LoadValueRGB();
}


void ColorType::ComponentChange(int delta)
{
    static const float maxs[4] = {0.0F, 255.0F, 255.0F, 255.0F};
    float * const pointers[4] = {0, &blue, &green, &red};
    int8 index = currentField;

    if (index >= 1 && index <= 3)
    {
        *(pointers[index]) += (float)Math::Sign(delta);
        Math::Limitation<float>(pointers[index], 0.0F, maxs[index]);
    }

    ApplyColor();

    SetBrightness();
}


Color& Color::operator=(const Color &color)
{
    value = color.value;
    return *this;
}


Color Color::Trig()
{
    Color result = Color::FILL;

    if(S_TRIG_SOURCE == ChA)
    {
        result = CHAN[ChA];
    }
    else // S_TRIG_SOURCE == TrigSource::B
    {
        result = CHAN[ChB];
    }

    return result;
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


uint Color::Div(uint color, int div)
{
    uint8 r = R_FROM_COLOR(color);
    uint8 g = G_FROM_COLOR(color);
    uint8 b = B_FROM_COLOR(color);

    return MAKE_COLOR(r / div, g / div, b / div);
}
