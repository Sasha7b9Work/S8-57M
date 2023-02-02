// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/Settings.h"


void PageDebug::PageTests::OnPress_Run()
{
    S_DBG_RUN_TEST = true;
    set.Save();
    Device::Reset();
}


DEF_BUTTON( bRun,                                                                                                                                       //--- ОТЛАДКА - ТЕСТЫ - Выполнить ---
    "Выполнить",
    "Выполнение выбранных тестов",
    &PageDebug::PageTests::self, Button::Active, PageDebug::PageTests::OnPress_Run
)


DEF_EMPTY_ITEM(e2, &PageDebug::PageTests::self)
DEF_EMPTY_ITEM(e3, &PageDebug::PageTests::self)
DEF_EMPTY_ITEM(e4, &PageDebug::PageTests::self)
DEF_EMPTY_ITEM(e5, &PageDebug::PageTests::self)


DEF_PAGE_5(pTests,                                                                                                                                                 //--- ОТЛАДКА - ТЕСТЫ ---
    "ТЕСТЫ",
    "Выполнение тестов",
    &bRun,
    &e2,
    &e3,
    &e4,
    &e5,
    PageName::Debug_Tests,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)


const Page *const PageDebug::PageTests::self = (const Page *)&pTests;
