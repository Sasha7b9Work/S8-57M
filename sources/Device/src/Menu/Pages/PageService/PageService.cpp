#include "defines.h"
#include "device.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


void PageService::OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::InModeOsci())
    {
        Osci::Start(true);
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- ������ - ����� �������� ---
    "����� �����",
    "����� �������� �� ��������� �� ���������",
    &PageService::self, Item::Active, PageService::OnPress_ResetSettings
)


DEF_BUTTON(bAutoSearch,                                                                                                                                     //--- ������ - ����� ������� ---
    "����� ����",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    &PageService::self, Item::Active, Osci::RunAutoSearch
)


DEF_CHOICE_4( cSoundVolume,
    "���������",
    "��������� ������ ���������",
    "����",
    "���",
    "�����",
    "����",
    S_SERV_ENUM_VOLUME, &PageService::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnChange_TimeDisable(int x, int y)
{
    Text("���").Draw(x + 36, y + 13, Color::BACK);
}

DEF_GOVERNOR( gTimeDisableDisplay,
    "��.����",
    "����� ���������� �������",
    S_SERV_TIME_DISABLE_DISPLAY, 0, 240, &PageService::self, Item::Active, Governor::Changed, OnChange_TimeDisable
)


DEF_PAGE_8_VAR( pService,                                                                                                                                                    //--- ������ --- //-V2567
    "������",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    &bResetSettings,
    &bAutoSearch,
    PageService::Calibrate::self,
    &cSoundVolume,
    PageRTC::self,
    &gTimeDisableDisplay,
    PageService::Information::self,
    &Item::empty,
    PageName::Service, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::self = static_cast<const Page *>(&pService);


void PageService::DecodePassword(const KeyEvent &event) //-V2506
{
#define NUM_SYMBOLS 10

    static const Key::E password[NUM_SYMBOLS] =
    {
        Key::F1, Key::F1,
        Key::F2, Key::F2,
        Key::F3, Key::F3,
        Key::F4, Key::F4,
        Key::F5, Key::F5
    };

    // ����� ��������� ��������
    static int charsMatch = 0;

    if(Menu::IsShown())
    {
        charsMatch = 0;
        return;
    }

    if(event.type != TypePress::Release)
    {
        return;
    }

    if(password[charsMatch++] == event.key)
    {
        if(charsMatch == NUM_SYMBOLS)
        {
            EnablePageDebug();
        }
    }
    else
    {
        charsMatch = 0;
    }
}


void PageService::EnablePageDebug()
{
    Page *page = const_cast<Page *>(&pService); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    items[7] = const_cast<Page *>(PageDebug::self); //-V2563 //-V2567
}


void PageService::DisablePageDebug()
{
    Page *page = const_cast<Page *>(&pService); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    items[7] = &Item::empty; //-V2563
}
