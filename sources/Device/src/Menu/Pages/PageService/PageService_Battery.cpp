// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cShow,                                                                                                                                  //--- СЕРВИС - БАТАРЕЯ - Показывать ---  
    "Показывать",
    "Включает/отключает вывод иноформации о напряжении батарии на экран",
    DISABLE_RU,
    ENABLE_RU,
    S_SERV_SHOW_INFO_VOLTAGE, &PageService::Battery::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_EMPTY_ITEM(e22, &PageService::Battery::self)
DEF_EMPTY_ITEM(e32, &PageService::Battery::self)
DEF_EMPTY_ITEM(e42, &PageService::Battery::self)
DEF_EMPTY_ITEM(e52, &PageService::Battery::self)


DEF_PAGE_5( pBattery,                                                                                                                                              //--- СЕРВИС - БАТАРЕЯ ---
    "БАТАРЕЯ",
    "Функции работы с аккумуляторами",
    &cShow,
    &e22,
    &e32,
    &e42,
    &e52,
    PageName::Service_Battery, &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageService::Battery::self = (const Page *)&pBattery;
