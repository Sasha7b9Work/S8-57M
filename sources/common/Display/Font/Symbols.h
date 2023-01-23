// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Font/Font.h"


struct Symbol8
{
    enum E
    {

    };
};


#ifdef DELETE
#undef DELETE
#endif

struct SymbolUGO2
{
    enum E
    {
        BIG_M           = '\x60',   // Большая буква "М"
        ARROW_RIGHT     = '\x64',
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',   // Символ флешки внизу экрана
        ETHERNET        = '\x82',   // Символ Ethernet внизу экрана
        USB             = '\x84',   // Символ USB внизу экрана
        MATH_FUNC_MUL   = '\x8a',   // МАТЕМАТИКА-ФУНКЦИЯ-УМНОЖЕНИЕ
        SELECT          = '\xa0'    // Символ выбора
    };
};


class Char
{
public:
    explicit Char(char ch, TypeFont::E font = TypeFont::Normal, int num = 1);
    int Draw(int x, int y, Color color = Color::Count) const;
    char ToChar() const { return (char)ch; }
protected:
    const uint8 ch;
    const TypeFont::E font;
    const int num_symbols;        // Может быть 1, 2, 4 или 10
private:
    void Draw2Symbols(int x, int y) const;
    void Draw4Symbols(int x, int y) const;
    void Draw10Symbols(int x, int y) const;
};


class SymbolUGO : public Char
{
public:
    enum E
    {
        RSHIFT_NORMAL,      // Символ смещения по напряжению нормальный
        TRIGLEV_NORMAL,     // Символ уровень синхронизации нормальный
        PLAY,               // Символ работы справа вверху экрана
        GOVERNOR_SHIFT_0,   // Символ регулятора
        GOVERNOR_SHIFT_1,
        GOVERNOR_SHIFT_2,
        GOVERNOR_SHIFT_3,
        GOVERNOR_LEFT,
        GOVERNOR_RIGHT,
        TSHIFT_NORMAL,      // Изображение смещения по времени
        CROSS,              // Символ диагонального креста
        COUPLE_GND,
        COUPLE_AC,
        COUPLE_DC,
        FRONT_FALL,
        FRONT_RISE,
        FILTR_HI,
        FILTR_LOW,
        FILTR_FULL,
        START_MODE_AUTO,
        START_MODE_HAND,
        START_MODE_SINGLE,
        TPOS,
        F_T,
        ARROW_UP,
        ARROW_DOWN,
        TSHIFT_LEFT,        // Изображение смещения по времени при переходе за левую границу экрана
        TSHIFT_RIGHT,       // Изображение смещения по времени при переходе за правую границу экрана
        TSHIFT_NORMAL_BACK, // Фон для изображения смещения по времени
        TPOS_1,             // 2-я часть символа TPos
        TPOS_2,             // 1-я часть символа TPos
        TRIGLEV_LOWER,      // Символ уровень синхронизации ниже сетки
        TRIGLEV_ABOVE,      // Символ уровень синхронизации выше сетки
        PEAK_DET_LEFT,      // Левая часть значка пикового детектора
        DELETE,             // ВНЕШН ЗУ-МАСКА-УДАЛИТЬ
        INSERT,             // ВНЕШН ЗУ-ИМЯ-ВСТАВИТЬ
        ARROW_LEFT,
        SAVE_TO_MEM,
        ROM,                // Микросхемка
        FLASH_DRIVE_BIG,    // Символ флешки
        DELETE_2,
        FOLDER,             // Каталог
        LEAVE,              // Значок выхода
        ENTER,              // Значок "Enter"
        TRIANGLE_UP,        // Стрелка треугольником вверх
        TRIANGLE_DOWN,      // Стрекла треугольником вниз
        SETTINGS,           // Значок настроек
        TRIANGLE_LEFT,      // Стрелка треугольником влево
        TRIANGLE_RIGHT,     // Стрелка треугольником вправо
        Count
    };

    SymbolUGO(E);

private:

    struct SymbolStruct
    {
        uint8 code;
        int num_symbols;
    };

    static const SymbolStruct desc[Count];
};
