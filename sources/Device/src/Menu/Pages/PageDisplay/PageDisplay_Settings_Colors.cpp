// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


void PageDisplay::Settings::Colors::Init()
{
    colorTypeA.Init();
    colorTypeB.Init();
    colorTypeGrid.Init();
}

static void OnPress_ResetColors()
{
    Color::raw[3] = MAKE_COLOR(0x00, 0xdf, 0xff);
    Color::raw[4] = MAKE_COLOR(0x00, 0xff, 0x00);
    Color::raw[2] = MAKE_COLOR(0x60, 0x60, 0x60);
    PageDisplay::Settings::Colors::colorTypeA.ReInit();
    PageDisplay::Settings::Colors::colorTypeB.ReInit();
    PageDisplay::Settings::Colors::colorTypeGrid.ReInit();
    PageDisplay::Settings::Colors::colorTypeA.ApplyColor();
    PageDisplay::Settings::Colors::colorTypeB.ApplyColor();
    PageDisplay::Settings::Colors::colorTypeGrid.ApplyColor();
}

DEF_BUTTON( bReset,                                                                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить ---
    "Сбросить",
    "Сброс всех цветов на значения по умолчанию",
    &PageDisplay::Settings::Colors::self, Item::Active, OnPress_ResetColors
)


ColorType PageDisplay::Settings::Colors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[ChA])

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 ---
    "Канал 1",
    "Выбор цвета канала 1",
    PageDisplay::Settings::Colors::colorTypeA, &PageDisplay::Settings::Colors::self
)


ColorType PageDisplay::Settings::Colors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[ChB])

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 ---
    "Канал 2",
    "Выбор цвета канала 1",
    PageDisplay::Settings::Colors::colorTypeB, &PageDisplay::Settings::Colors::self
)


ColorType PageDisplay::Settings::Colors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---
    "Сетка",
    "Устанавливает цвет сетки",
    PageDisplay::Settings::Colors::colorTypeGrid, &PageDisplay::Settings::Colors::self
)


DEF_EMPTY_ITEM(a, &PageDisplay::Settings::Colors::self)


DEF_PAGE_5( pColors,                                                                                                                                    //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ---
    "ЦВЕТА",
    "Выбор цветов дисплея",
    &bReset,
    &gcChannelA,
    &gcChannelB,
    &gcGrid,
    &a,
    PageName::Display_Settings_Colors, &PageDisplay::Settings::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Settings::Colors::self = (const Page *)&pColors;
