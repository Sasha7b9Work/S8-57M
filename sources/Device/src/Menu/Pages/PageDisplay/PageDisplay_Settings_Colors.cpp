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
    GlobalColors[3] = MAKE_COLOR(0x00, 0xdf, 0xff);
    GlobalColors[4] = MAKE_COLOR(0x00, 0xff, 0x00);
    GlobalColors[2] = MAKE_COLOR(0x60, 0x60, 0x60);
    PageDisplay::Settings::Colors::colorTypeA.ReInit();
    PageDisplay::Settings::Colors::colorTypeB.ReInit();
    PageDisplay::Settings::Colors::colorTypeGrid.ReInit();
    PageDisplay::Settings::Colors::colorTypeA.ApplyColor();
    PageDisplay::Settings::Colors::colorTypeB.ApplyColor();
    PageDisplay::Settings::Colors::colorTypeGrid.ApplyColor();
}

DEF_BUTTON( bReset,                                                                                                                          //--- ������� - ��������� - ����� - �������� ---
    "��������",
    "����� ���� ������ �� �������� �� ���������",
    &PageDisplay::Settings::Colors::self, Item::Active, OnPress_ResetColors
)


ColorType PageDisplay::Settings::Colors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[ChanA])

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1",
    "����� ����� ������ 1",
    PageDisplay::Settings::Colors::colorTypeA, &PageDisplay::Settings::Colors::self
)


ColorType PageDisplay::Settings::Colors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::CHAN[ChanB])

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2",
    "����� ����� ������ 1",
    PageDisplay::Settings::Colors::colorTypeB, &PageDisplay::Settings::Colors::self
)


ColorType PageDisplay::Settings::Colors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ������� - ��������� - ����� - ����� ---
    "�����",
    "������������� ���� �����",
    PageDisplay::Settings::Colors::colorTypeGrid, &PageDisplay::Settings::Colors::self
)


DEF_PAGE_4( pColors,                                                                                                                                    //--- ������� - ��������� - ����� ---
    "�����",
    "����� ������ �������",
    &bReset,
    &gcChannelA,
    &gcChannelB,
    &gcGrid,
    PageName::Display_Settings_Colors, &PageDisplay::Settings::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Settings::Colors::self = static_cast<const Page *>(&pColors);
