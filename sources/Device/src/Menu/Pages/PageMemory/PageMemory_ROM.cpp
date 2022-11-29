#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Beeper.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


// ���������� ���� ������� ������
static void DrawMemoryCell(int num, bool exist);

static bool HandlerKey_ROM(const KeyEvent &event);



static void OnPress_Next()
{
    HandlerKey_ROM(KeyEvent(Key::Right, TypePress::Release));
}

static void Draw_Next(int x, int y)
{
    Char(SymbolUGO2::ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON
(
    bNext,
    "���������",
    "������� � ���������� �������",
    &PageROM::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
    HandlerKey_ROM(KeyEvent(Key::Left, TypePress::Release));
}

static void Draw_Prev(int x, int y)
{
    Char(SymbolUGO2::ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON
(
    bPrev,
    "����������",
    "������� � ����������� �������",
    &PageROM::self, Item::Active, OnPress_Prev, Draw_Prev
)


static void OnPress_Delete()
{
    Display::Message::Show("������ ���������� ������", false);

    ROM::Data::Erase(NUM_ROM_SIGNAL);

    Display::Message::Hide();

    Color::ChangeFlash(true);
}

static void Draw_Delete(int x, int y)
{
    Char(SymbolUGO2::DELETE).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON                                                                                                                                        //--- ������ - ����� �� - ������� ---
(
    bDelete,
    "�������",
    "������� ��������� ������ �� ����������� ������������� ����������",
    &PageROM::self, Item::Active, OnPress_Delete, Draw_Delete
)


static void OnPress_Save()
{
    Display::Message::Show("��������� � ������", false);

    const DataSettings *ds = RAM::Get();

    if (ds)
    {
        ROM::Data::Save(NUM_ROM_SIGNAL, ds);
    }

    Display::Message::Hide();

    Color::ChangeFlash(true);
}

static void Draw_SaveToMemory(int x, int y)
{
    Char(SymbolUGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON                                                                                                                             //--- ������ - ����� �� - ��������� � ������ ---
(
    bSave,
    "��������� � ������",
    "��������� ������ �� ���������� ������������ ����������",
    &PageROM::self, Item::Active, OnPress_Save, Draw_SaveToMemory
)


static void OnPress_TypeSignal()
{
    Math::CircleIncrease<uint8>(reinterpret_cast<uint8*>(&S_MEM_TYPE_SIGNAL_ROM), 0, TypeSignalROM::Count - 1);
}

static void Draw_Recorded(int x, int y)
{
    Text("���").Draw(x + 1, y + 5);
}

static void Draw_Current(int x, int y)
{
    Text("���").Draw(x + 1, y + 5);
}

static void Draw_Both(int x, int y)
{
    Text("���").Draw(x + 1, y + 5);
}

static void Draw_TypeSignal(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw func[TypeSignalROM::Count] = { Draw_Recorded, Draw_Current, Draw_Both };
    func[S_MEM_TYPE_SIGNAL_ROM](x, y);
}

DEF_GRAPH_BUTTON_HINTS_3                                                                                                                            //--- ������ - ����� �� - ��� ������� ---
(
    bTypeSignal,
    "��� �������",
    "����� ������ ��������",
    &PageROM::self, Item::Active, OnPress_TypeSignal, Draw_TypeSignal,
    Draw_Recorded, "��������� ���������� ������ �� ������",
    Draw_Current, "��������� ������� ������",
    Draw_Both, "�������� ��� ������� ������������"
)


static void OnOpenClose_ROM(bool open)
{
    if(open)
    {
        Color::ChangeFlash(true);
    }

    S_MEM_MODE_WORK = open ? ModeWork::ROM : ModeWork::Dir;
}

static void AfterDraw_ROM()
{
    // ������ �������� ��������� ������

    bool exist[ROM::Data::MAX_NUM_SAVED_WAVES] = { false };

    ROM::Data::GetInfo(exist);

    for (int i = 0; i < ROM::Data::MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryCell(i, exist[i]);
    }
}

static void DrawMemoryCell(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 13;
    int width = 12;
    Region(width, 10).Fill(x, y, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);

    Color((num == NUM_ROM_SIGNAL) ? Color::FLASH_01 : Color::FILL).SetAsCurrent();

    if (exist)
    {
        Integer(num + 1).ToString(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        String(Symbol8::CROSS).Draw(x + 3, y + 1);
    }
}

static bool HandlerKey_ROM(const KeyEvent &event)
{
    bool result = false;

    if (event.IsRelease() || event.IsLong())
    {
        Beeper::RegulatorSwitchRotate();

        if (event.IsDecrease())
        {
            Math::CircleDecrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1); //-V2567

            Color::ChangeFlash(true);

            result = true;
        }
        else if (event.IsIncrease()) //-V2516
        {
            Math::CircleIncrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1); //-V2567

            Color::ChangeFlash(true);

            result = true;
        }       
    }

    return result;
}

static bool NormalTitle_ROM()
{
    return false;
}

DEF_PAGE_5                                                                                                                                                        //--- ������ - ����� �� --- //-V2567
(
    pROM,
    "����� ��",
    "������� � ����� ������ � ���������� �������",
    &bPrev,
    &bNext,
    &bSave,
    &bDelete,
    &bTypeSignal,
    PageName::Memory_Internal,
    &PageMemory::self, Item::Active, NormalTitle_ROM, OnOpenClose_ROM, AfterDraw_ROM, HandlerKey_ROM
)

const Page *const PageROM::self = static_cast<const Page *>(&pROM);
