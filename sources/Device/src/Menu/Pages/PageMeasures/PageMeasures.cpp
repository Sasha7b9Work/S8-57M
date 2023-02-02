// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


bool MeasuresModeViewSignals::IsCompress()
{
    return S_MEAS_MODE_VIEW_SIGNALS_IS_COMPRESS;
}


bool MeasuresOnDisplay::IsVertical()
{
    return (S_MEAS_ON_DISPLAY_IS_6_1 || S_MEAS_ON_DISPLAY_IS_6_2);
}


bool CursorsControl::IsDisabledT()
{
    return S_CURS_CONTROL_T(S_CURS_SOURCE) == Disabled;
}


bool CursorsControl::IsEnabled1T()
{
    return (S_CURS_CONTROL_T(S_CURS_SOURCE) == _1 || S_CURS_CONTROL_T(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsEnabled2T()
{
    return (S_CURS_CONTROL_T(S_CURS_SOURCE) == _2 || S_CURS_CONTROL_T(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsDisabledU()
{
    return S_CURS_CONTROL_U(S_CURS_SOURCE) == Disabled;
}


bool CursorsControl::IsEnabled1U()
{
    return (S_CURS_CONTROL_U(S_CURS_SOURCE) == _1 || S_CURS_CONTROL_U(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsEnabled2U()
{
    return (S_CURS_CONTROL_U(S_CURS_SOURCE) == _2 || S_CURS_CONTROL_U(S_CURS_SOURCE) == Both);
}


DEF_EMPTY_ITEM(e3, &PageMeasures::self)
DEF_EMPTY_ITEM(e4, &PageMeasures::self)
DEF_EMPTY_ITEM(e5, &PageMeasures::self)


DEF_PAGE_5( pMeasures,                                                                                                                                                    //--- ��������� ---
    "���������",
    "������ � ���������� ��������� - ��������� � ��������������",
    PageCursorsMeasures::self,
    PageAutoMeasures::self,
    &e3,
    &e4,
    &e5,
    //PageMath::self,
    PageName::Measures, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page *const PageMeasures::self = (const Page *)&pMeasures;
