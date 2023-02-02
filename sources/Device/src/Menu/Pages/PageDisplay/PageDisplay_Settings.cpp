// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"



static void OnChanged_Brightness()
{
    Display::LoadBrightness();
}

void PageDisplay::SetBrightness(int value)
{
    Math::Limitation(&value, 0, 100);
    set.disp._brightness = (uint8)(value);
    OnChanged_Brightness();
}


DEF_GOVERNOR( gBrightness,                                                                                                                            //--- ������� - ��������� - ������� ---
    "�������",
    "��������� ������� �������� �������",
    S_DISP_BRIGHTNESS, 0, 100, &PageDisplay::Settings::self, Item::Active, OnChanged_Brightness, Governor::AfterDraw
)


static void OnChanged_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6( cAutoHide,                                                                                                                             //--- ������� - ��������� - �������� ---
    "��������",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "�������",
    "����� 5 ���",
    "����� 10 ���",
    "����� 15 ���",
    "����� 30 ���",
    "����� 60 ���",
    S_MENU_AUTOHIDE, &PageDisplay::Settings::self, Item::Active, OnChanged_AutoHide, Choice::AfterDraw
)


DEF_EMPTY_ITEM(e411, &PageDisplay::Settings::self)
DEF_EMPTY_ITEM(e511, &PageDisplay::Settings::self)


DEF_PAGE_5( pSettings,                                                                                                                                          //--- ������� - ��������� ---
    "���������",
    "�������������� ��������� �������",
    PageDisplay::Settings::Colors::self,
    &gBrightness,
    &cAutoHide,
    &e411,
    &e511,
    PageName::Display_Settings, &PageDisplay::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Settings::self = (const Page *)&pSettings;
