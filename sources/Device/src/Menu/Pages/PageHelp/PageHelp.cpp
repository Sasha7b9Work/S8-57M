#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/HelpContent.h"
#include "Menu/Pages/Include/PageHelp.h"


static void Draw_Enter(int x, int y)
{
    Char(SymbolUGO2::ENTER).Draw4SymbolsInRect(x + 2, y + 2);
}


static void Help_Draw_Leave(int x, int y)
{
    Char(SymbolUGO2::LEAVE).Draw4SymbolsInRect(x + 2, y + 1);
}


static void Draw_Prev(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 5);
}


static void Help_Draw_Next(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 5);
}


static bool HandlerKey_Help(const KeyEvent &)
{
    return false;
}


DEF_GRAPH_BUTTON( bEnter,                                                                                                                                          //--- ������ - ������� ---
    "�������",
    "��������� ������ �������",
    &PageHelp::self, HelpContent::EnterParagraphIsActive, HelpContent::EnterParagraph, Draw_Enter
)


DEF_GRAPH_BUTTON( bHelpLeave,                                                                                                                                          //--- ������ - ������� ---
     "�������",
     "��������� ������ �������",
    &PageHelp::self, HelpContent::LeaveParagraphIsActive, HelpContent::LeaveParagraph, Help_Draw_Leave
)


DEF_GRAPH_BUTTON( bPrev,                                                                                                                                 //--- ������ - ���������� ������ ---
    "���������� ������",
    "������� ���������� ������ �������",
    &PageHelp::self, Item::Active, HelpContent::PrevParagraph, Draw_Prev
)


DEF_GRAPH_BUTTON( bHelpNext,                                                                                                                                  //--- ������ - ��������� ������ ---
    "��������� ������",
    "������� ��������� ������ �������",
    &PageHelp::self, Item::Active, HelpContent::NextParagraph, Help_Draw_Next
)

DEF_PAGE_4( pHelp,                                                                                                                                                           //--- ������ --- //-V2567
    "������",
    "������� ������� ������",
    &bEnter,
    &bHelpLeave,
    &bPrev,
    &bHelpNext,
    PageName::Help, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, HelpContent::Draw, HandlerKey_Help
)

const Page *const PageHelp::self = static_cast<const Page *>(&pHelp);
