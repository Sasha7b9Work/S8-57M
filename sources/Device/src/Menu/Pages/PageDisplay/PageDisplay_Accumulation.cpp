// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"


DEF_CHOICE_9( cNumber,                                                                                                                            //--- ������� - ���������� - ���������� ---
    "����������"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ �������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\".",
    DISABLE_RU,
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "�������������",
    S_DISP_ENUM_ACCUM, &PageDisplay::Accumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::Accumulation::OnPress_Accumulation_Clear()
{
    DisplayOsci::SetFlagRedraw();
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ������� - ���������� - �������� ---
    "��������",
    "������� ����� �� ����������� ��������.",
    &PageDisplay::Accumulation::self, IsActive_Accum_Clear, PageDisplay::Accumulation::OnPress_Accumulation_Clear
)



static bool IsActive_Accum()
{
    return !OSCI_IN_MODE_RANDOMIZER;
}


DEF_EMPTY_ITEM(e30, &PageDisplay::Accumulation::self)
DEF_EMPTY_ITEM(e40, &PageDisplay::Accumulation::self)
DEF_EMPTY_ITEM(e50, &PageDisplay::Accumulation::self)


DEF_PAGE_5( pAccum,                                                                                                                           //--- ������� - ���������� ---
    "����������",
    "��������� ������ ����������� ��������� �������� �� ������",
    &cNumber,
    &bClear,
    &e30,
    &e40,
    &e50,
    PageName::Display_Accumulation, &PageDisplay::self, IsActive_Accum, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Accumulation::self = (const Page *)&pAccum;
