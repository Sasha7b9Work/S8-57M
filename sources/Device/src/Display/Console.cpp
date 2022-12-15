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
    // true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    bool inProcessDraw = false;
    // true, ���� ���������� ������� ���������� ������
    bool inProcessAddingString = false;
    // ���������� ����������� ����� � �������
    static int stringInConsole = 0;
    // ����� ����������� ���������� �������� ������������� ���������� ����� � �������
    static int16 prevMaxStrinsInConsole = -1;

    static void DeleteFirstString();

//    // ���������� true, ���� ������� ������ � � ��� ������ ����������� ������
//    static bool IsBusy();

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

    DFont::Set(DTypeFont::_5);

    int y = -1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = DFont::GetLengthText(buffer[i]);
        Region(length, 6).Fill(0, y + 3, Color::BACK);
        String(buffer[i]).Draw(1, y, Color::FILL);
        y += 6;
    }

    DFont::Set(DTypeFont::_8);

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


void Console::AddString(const char *string)
{
    // \todo �� ���������� ��������� ������. ������� ���������� ����������

    //if (!IsBusy())      // ���������� �� ������� ����, ��� ������ �� ���������� ����� ������� � ������ ������
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
    // \todo �����, ������, �� ������ ���������� ��������� � ������, ����� �������� ����� ������, ��� ����������� ���������� �� ����������

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
