// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include <cmath>
#include <cstring>



DEF_CHOICE_2( cShow,                                                                                                                                           //--- ������� - ���������� ---
    "����������",
    "��������/��������� �������.",
    "���",
    "��",
    S_CURS_SHOW, &PageCursorsMeasures::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

static bool IsActive_ShowFreq()
{
    return S_CURS_SHOW;
}

DEF_CHOICE_2( cShowFreq,                                                                                                                                             //--- ������� - 1/dT ---
    "1/dT"
    ,
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ "
    "�������� ����� ���������� ����� ���������� ���������."
    ,
    DISABLE_RU,
    ENABLE_RU,
    S_CURS_SHOW_FREQ, &PageCursorsMeasures::self, IsActive_ShowFreq, Choice::Changed, Choice::AfterDraw
)


DEF_EMPTY_ITEM(e49, &PageCursorsMeasures::self)
DEF_EMPTY_ITEM(e59, &PageCursorsMeasures::self)


DEF_PAGE_5( pCursors,                                                                                                                                                     //--- ��������� ---
    "�������",
    "��������� ���������",
    &cShow,
    &cShowFreq,
    PageCursorsMeasures::Set::self,
    &e49,
    &e59,
    PageName::CursorsMeasures, &PageMeasures::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageCursorsMeasures::self = (const Page *)&pCursors;
