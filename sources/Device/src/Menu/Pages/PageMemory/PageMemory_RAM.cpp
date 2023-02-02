// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "common/Display/Font/Symbols.h"
#include "Hardware/Memory/RAM.h"
#include "Menu/MenuItems.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


static void OnPress_Next()
{
    Math::CircleIncrease<int16>(&RAM::currentSignal, 0, (int16)RAM::NumberDatas() - 1);
}

static void Draw_Next(int x, int y)
{
    SymbolUGO(SymbolUGO::ARROW_RIGHT).Draw(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bNext,                                                                                                                             //--- ѕјћя“№ - ѕќ—Ћ≈ƒЌ»≈ - —ледующий ---
    "—ледующий",
    "ѕерейти к следующему сигналу",
    &PageRAM::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
    Math::CircleDecrease<int16>(&RAM::currentSignal, 0, (int16)RAM::NumberDatas() - 1);
}

static void Draw_Prev(int x, int y)
{
    SymbolUGO(SymbolUGO::ARROW_LEFT).Draw(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bPrev,                                                                                                                            //--- ѕјћя“№ - ѕќ—Ћ≈ƒЌ»≈ - ѕредыдущий ---
    "ѕредыдущий",
    "ѕерейти к предыдущему сигналу",
    &PageRAM::self, Item::Active, OnPress_Prev, Draw_Prev
)

static void AfterDraw_RAM(int num_field)
{
    if (num_field == 0)
    {
        int width = 76;
        int height = 19;
        int x0 = Grid::Right() - width + 1;
        Region(width, height).Fill(Grid::Right() - width, Grid::Top(), Color::BACK);
        Rectangle(width, height).Draw(Grid::Right() - width, Grid::Top(), Color::FILL);
        int y = Grid::Top() + 3;
        Integer(RAM::currentSignal + 1).ToString(false, 3).Draw(x0 + 2, y);
        String("/").Draw(x0 + 33, y);
        Integer((int)(RAM::NumberDatas())).ToString(false, 3).Draw(x0 + 45, y);
    }
}

static void OnOpenClose_RAM(bool enter)
{
    RAM::canTrace = true;

    if (enter)
    {
        RUN_FPGA_BEFORE_SB = Osci::IsRunning() ? 1U : 0U;
        Osci::Stop();
        S_MEM_MODE_WORK = ModeWork::RAM;
        RAM::currentSignal = 0;
        Display::AdditionalFunctionDraw::Set(AfterDraw_RAM);
    }
    else
    {
        Display::AdditionalFunctionDraw::Remove();

        S_MEM_MODE_WORK = ModeWork::Dir;

        if (RUN_FPGA_BEFORE_SB)
        {
            Osci::Start(true);
        }
    }
}

static bool HandlerKey_RAM(const KeyEvent &)
{
    return false;
}


DEF_EMPTY_ITEM(e35, &PageRAM::self)
DEF_EMPTY_ITEM(e45, &PageRAM::self)
DEF_EMPTY_ITEM(e55, &PageRAM::self)


DEF_PAGE_5(pRAM,                                                                                                                                                //--- ѕјћя“№ - ѕќ—Ћ≈ƒЌ»≈ ---
    "ѕќ—Ћ≈ƒЌ»≈",
    "ѕереход в режим работы с последними полученными сигналами",
    &bPrev,
    &bNext,
    &e35,
    &e45,
    &e55,
    PageName::Memory_Last,
    &PageMemory::self, Item::Active, Page::NormalTitle, OnOpenClose_RAM, HandlerKey_RAM
)

const Page *const PageRAM::self = (const Page *)&pRAM;
