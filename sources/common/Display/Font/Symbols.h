// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Font/Font.h"


struct Symbol8
{
    enum E
    {
        TSHIFT_NORMAL       = '\x84',  // Изображение смещения по времени
        TSHIFT_NORMAL_BACK  = '\x85',  // Фон для изображения смещения по времени
        TSHIFT_LEFT         = '\x86',  // Изображение смещения по времени при переходе за левую границу экрана
        TSHIFT_RIGHT        = '\x87',  // Изображение смещения по времени при переходе за правую границу экрана
        CROSS               = '\x88',  // Символ диагонального креста
        TPOS_1              = '\x8a',  // 2-я часть символа TPos
        TPOS_2              = '\x8b',  // 1-я часть символа TPos
        TRIGLEV_LOWER       = '\x95',  // Символ уровень синхронизации ниже сетки
        TRIGLEV_ABOVE       = '\x96',  // Символ уровень синхронизации выше сетки
        ARROW_UP            = '\xb9',
        ARROW_DOWN          = '\x9d'
    };
};


#ifdef DELETE
#undef DELETE
#endif

struct SymbolUGO2
{
    enum E
    {
        PEAK_DET_LEFT   = '\x12',   // Левая часть значка пикового детектора
        PEAK_DET_RIGHT  = '\x13',   // Правая часть значка пикового детектора
        ARROW_LEFT      = '\x20',
        DELETE          = '\x22',   // ВНЕШН ЗУ-МАСКА-УДАЛИТЬ
        INSERT          = '\x26',   // ВНЕШН ЗУ-ИМЯ-ВСТАВИТЬ
        SAVE_TO_MEM     = '\x2c',
        ROM             = '\x40',   // Микросхемка
        FLASH_DRIVE_BIG = '\x42',   // Символ флешки
        FOLDER          = '\x46',   // Каталог
        LEAVE           = '\x48',   // Значок выхода
        ENTER           = '\x4a',   // Значок "Enter"
        TRIANGLE_UP     = '\x4c',   // Стрелка треугольником вверх
        TRIANGLE_DOWN   = '\x4e',   // Стрекла треугольником вниз
        BIG_M           = '\x60',   // Большая буква "М"
        SETTINGS        = '\x62',   // Значок настроек
        ARROW_RIGHT     = '\x64',
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',   // Символ флешки внизу экрана
        ETHERNET        = '\x82',   // Символ Ethernet внизу экрана
        USB             = '\x84',   // Символ USB внизу экрана
        MATH_FUNC_MUL   = '\x8a',   // МАТЕМАТИКА-ФУНКЦИЯ-УМНОЖЕНИЕ
        TRIANGLE_LEFT   = '\x8c',   // Стрелка треугольником влево
        TRIANGLE_RIGHT  = '\x8e',   // Стрелка треугольником вправо
        SELECT          = '\xa0'    // Символ выбора
    };
};


class Char
{
public:
    explicit Char(char ch, TypeFont::E font = TypeFont::Normal, int num = 1);
    int Draw(int x, int y, Color color = Color::Count) const;
    char ToChar() const { return ch; }
protected:
    const char ch;
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
