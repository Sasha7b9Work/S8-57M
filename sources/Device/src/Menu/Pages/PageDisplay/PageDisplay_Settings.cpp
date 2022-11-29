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
    set.disp._brightness = static_cast<uint8>(value);
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


DEF_PAGE_3( pSettings,                                                                                                                                          //--- ������� - ��������� --- //-V2567
    "���������",
    "�������������� ��������� �������",
    PageDisplay::Settings::Colors::self,
    &gBrightness,
    &cAutoHide,
    PageName::Display_Settings, &PageDisplay::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::Settings::self = static_cast<const Page *>(&pSettings);
