// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cShow,                                                                                                                                 //--- ������� - ������� - ���������� ---
    "����������",
    "",
    "���",
    "��",
    S_DBG_SHOW_CONSOLE, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_GOVERNOR( gNumStrings,                                                                                                                          //--- ������� - ������� - ����� ����� ---
    "����� �����",
    "",
    S_DBG_NUM_STRINGS_IN_CONSOLE, 0, 33, &PageDebug::PageConsole::self, Item::Active, Console::OnChanged_MaxStringsInConsole, Governor::AfterDraw
)


DEF_CHOICE_2( cShowAll,                                                                                                               //--- ������� - ������� - �������� - ���������� ��� ---
    "���������� ���",
    "���������� ��� ��������, ���������� � ��������",
    "���",
    "��",
    S_DBG_SHOW_ALL_REGISTERS, &PageDebug::PageRegisters::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Registers()
{
    return S_DBG_SHOW_ALL_REGISTERS;
}

DEF_CHOICE_2( cRD_FL,                                                                                                                          //--- ������� - ������� - �������� - RD_FL ---
    "RD_FL",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_FLAG, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftA,                                                                                                                     //--- ������� - ������� - �������� - U ��. 1� ---
    "U ��. 1�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RSHIFT(ChanA), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftB,                                                                                                                     //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RSHIFT(ChanB), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigLev,                                                                                                                     //--- ������� - ������� - �������� - U �����. ---
    "U �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_TRIG_LEV, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeA,                                                                                                                   //--- ������� - ������� - �������� - �����/��� 1 ---
    "�����/��� 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RANGE(ChanA), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeB,                                                                                                                   //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RANGE(ChanB), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigParam,                                                                                                              //--- ������� - ������� - �������� - �����. �����. ---
    "�����. �����.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_TRIG_PARAM, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamA,                                                                                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_CHAN_PARAM(ChanA), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamB,                                                                                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_CHAN_PARAM(ChanB), &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTBase,                                                                                                                      //--- ������� - ������� - �������� - �����/��� ---
    "�����/���",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_TBASE, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTShift,                                                                                                                         //--- ������� - ������� - �������� - � ��. ---
    "� ��.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_TSHIFT, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_12( pRegisters,                                                                                                                               //--- ������� - ������� - �������� ---
    "��������",
    "",
    &cShowAll,
    &cRD_FL,
    &cRShiftA,
    &cRShiftB,
    &cTrigLev,
    &cRangeA,
    &cRangeB,
    &cTrigParam,
    &cChanParamA,
    &cChanParamB,
    &cTBase,
    &cTShift,
    PageName::Debug_Console_Registers, &PageDebug::PageConsole::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageRegisters::self = (const Page *)&pRegisters;


DEF_CHOICE_2( cShowGate,
    "������",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RAND_GATES, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_CHOICE_2( cShowPredPost,
    "����������",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_DBG_SHOW_RAND_PRED_POST, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_4( pConsole,                                                                                                                                             //--- ������� - ������� ---
    "�������",
    "��������� ������������ � ����������� ���������� �������",
    &cShow,
    &gNumStrings,
    &cShowGate,
    &cShowPredPost,
    PageName::Debug_Console, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageConsole::self = (const Page *)&pConsole;
