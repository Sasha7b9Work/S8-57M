// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/SettingsNRST.h"


static int16 shift1 = setNRST._exShift[0][0];
static int16 shift2 = setNRST._exShift[1][0];


static void OnChange_Shift1()
{
    setNRST.SetExtraShift((int8)shift1);
}

DEF_GOVERNOR( gExShift1,
    "������ 1",
    "",
    shift1, -100, 100, &PageDebug::PageADC::self, Item::Active, OnChange_Shift1, Governor::AfterDraw
)


static void OnChange_Shift2()
{
    setNRST.SetExtraShift((int8)shift2);
}


DEF_GOVERNOR( gExShift2,
    "������ 2",
    "",
    shift2, -100, 100, &PageDebug::PageADC::self, Item::Active, OnChange_Shift2, Governor::AfterDraw
)


DEF_EMPTY_ITEM(e36, &PageDebug::PageADC::self)
DEF_EMPTY_ITEM(e46, &PageDebug::PageADC::self)
DEF_EMPTY_ITEM(e56, &PageDebug::PageADC::self)


DEF_PAGE_5( pADC,
    "���",
    "",
    &gExShift1,
    &gExShift2,
    &e36,
    &e46,
    &e56,
    PageName::Debug_ADC, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::self = (const Page *)&pADC;
