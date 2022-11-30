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


DEF_BUTTON( bRun,                                                                                                                                       //--- ������� - ����� - ��������� ---
    "���������",
    "���������� ��������� ������",
    &PageDebug::PageTests::self, Button::Active, PageDebug::PageTests::OnPress_Run
)


DEF_PAGE_1( pTests,                                                                                                                                                 //--- ������� - ����� --- //-V2567
    "�����",
    "���������� ������",
    &bRun,
    PageName::Debug_Tests,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageDebug::PageTests::self = static_cast<const Page *>(&pTests);
