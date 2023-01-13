// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Console.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cstdio>


namespace Console
{
    static CHAR_BUF2(buffer, 33, 100);

    // true означает, что идёт процесс вывода консоли и добавлять в неё новые строки нельзя (это происходит, когда добавление идёт из прерывания)
    bool inProcessDraw = false;

    // true, если происходит процесс добавления строки
    bool inProcessAddingString = false;

    // Количество заполненных строк в консоли
    static int stringInConsole = 0;

    // Здесь сохраняется предыдущее значение максимального количества строк в консоли
    static int16 prevMaxStrinsInConsole = -1;

    static void DeleteFirstString();
}


void Console::Init()
{
    inProcessDraw = false;
    inProcessAddingString = false;
    stringInConsole = 0;
    prevMaxStrinsInConsole = -1;
}


void Console::Draw()
{
    if (prevMaxStrinsInConsole == -1)
    {
        prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;
    }

    if (!IsShown() || stringInConsole == 0)
    {
        return;
    }

    inProcessDraw = true;

    Font::Set(TypeFont::Small);

    int y = 70;
    int h = 10;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = Font::GetLengthText(buffer[i]);
        Region(length + 1, h).Fill(0, y, Color::BACK);
        String(buffer[i]).Draw(2, y + 1, Color::FILL);
        y += h;
    }

    Font::Set(TypeFont::Normal);

    inProcessDraw = false;
}


void Console::DeleteFirstString()
{
    for (int16 i = 1; i < stringInConsole; i++)
    {
        std::strcpy(buffer[i - 1], buffer[i]);
    }
    stringInConsole--;
}


void Console::AddString(pchar string)
{
    // \todo Мы пропускаем некоторые строки. Сделать отложенное добавление

    //if (!IsBusy())      // Страхуемся на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        inProcessAddingString = true;

        static int count = 0;
        if (stringInConsole == S_DBG_NUM_STRINGS_IN_CONSOLE)
        {
            DeleteFirstString();
        }
        std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;

        inProcessAddingString = false;
    }
}


bool Console::IsShown()
{
    return (S_DBG_SHOW_CONSOLE != 0);
}


void Console::OnChanged_MaxStringsInConsole()
{
    // \todo Здесь, видимо, не совсем корректное поведение в случае, когда реальных строк меньше, чем максимально допустимое их количество

    int delta = prevMaxStrinsInConsole - S_DBG_NUM_STRINGS_IN_CONSOLE;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;


}


//bool Console::IsBusy()
//{
//    return inProcessDraw || inProcessAddingString;
//}
