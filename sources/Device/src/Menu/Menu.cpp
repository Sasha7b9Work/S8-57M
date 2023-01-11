// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Display/Primitives.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/HandlersKeys.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include <cstdio>


static const Page *const pages[] =
{
    PageFunction::self,
    PageMeasures::self,
    PageMemory::self,
    PageDisplay::self,
    PageService::self,
    PageTime::self,
    PageTrig::self,
    PageChannelA::self,
    PageChannelB::self,
    nullptr
};


namespace Menu
{
    // ��������� �������� ������� �� ������ �������� page
    static Item *LastOpened(Page *page);

    // ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();

    static void ProcessButtonForHint(Key::E button);

    // �������� ���������
    static void DrawHint();

    // ������� ��� ��������, ������� �� ����� ���� ��������� ��� ���������
    static void CloseAllBadOpenedPages();

    // ������� parent, ���� �� �������� ���������� page
    static void CloseIfSubPage(Page *parent, Page *page);

    static void ClosePage(Page *page);

    static void DrawHintItem(int x, int y, int width);

    // ����, ��� �������� ����� �������� ���������
    Item *itemHint = nullptr;

    // ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    pchar stringForHint = nullptr;
}


void Menu::Update()
{
    static uint lastModified = 0;
    static bool saved = true;

    while(!BufferButtons::IsEmpty())                // ���� ���� ������� ����������
    {
        KeyEvent event = BufferButtons::Extract();  // ��������� ��������� �������

        if (!Keyboard::KeyIsActive(event.key))      // ���� ������ �� ��������� ��� ��������� ������:
        {
            continue;                               // �������� � ���������
        }

        if (HINT_MODE_ENABLED)                      // ���� ������� ����� ���������
        {
            ProcessButtonForHint(event.key);        // �� ������� ��������� ��� ��������������� ������
            continue;                               // � ��������� � ��������� ������
        }

        Handlers::Process(event);                   // �� ������������ ���

        if(event.key != Key::Start)
        {
            DisplayOsci::SetFlagRedraw();
        }

        lastModified = TIME_MS;
        saved = false;
    }

    HAL_BUS::ConfigureToFSMC();

    if (!saved && TIME_MS - lastModified > 5000)
    {
        Settings::Save();
        setNRST.Save();
        saved = true;
    }
}


void Menu::ProcessButtonForHint(Key::E button)
{
    if (button == Key::Enter)
    {
        stringForHint =
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� "
            "�������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� "
            "��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������.";
    } 
    else if (button == Key::Display)
    {
        stringForHint = "������ ������� ��������� ���� �������� �������.";
    }
    else if (button == Key::Memory)
    {
        stringForHint =
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����.";
    }
    else if (button == Key::Measure)
    {
        stringForHint = "������ ����� ��������� ���� �������������� ���������.";
    }
    else if (button == Key::Service)
    {
        stringForHint = "������ ������ ��������� ���� ��������� ������������.";
    }
    else if (button == Key::Start)
    {
        stringForHint = "������ ����/�TO� ��������� � ������������� ������� ����� ����������.";
    }
    else if (button == Key::ChannelA)
    {
        stringForHint =
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�.";
    }
    else if (button == Key::ChannelB)
    {
        stringForHint =
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�.";
    }
    else if (button == Key::Time)
    {
        stringForHint =
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�.";
    }
    else if (button == Key::Trig)
    {
        stringForHint =
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� "
            "��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� "
            "������������� 0�.";
    }
}


void Menu::SetAutoHide(bool)
{
    if (Menu::IsShown())
    {
        if (Display::TimeMenuAutoHide() == 0)
        {
            Timer::Disable(TypeTimer::MenuAutoHide);
        }
        else
        {
            Timer::SetAndStartOnce(TypeTimer::MenuAutoHide, OnTimerAutoHide, (uint)(Display::TimeMenuAutoHide()));
        }
    }
}


void Menu::OnTimerAutoHide()
{
    Menu::Hide();
}


void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(Menu::OpenedItem() == PageDrive::Manager::self)
        {
            OpenedItem()->HandlerFX(TypePress::Release);
        }
    }
    else if(S_MEM_FLASH_AUTO_CONNECT)
    {
        PageMemory::OnOpenClose_Drive_Manager();
    }
}


void Menu::Show()
{
    S_MENU_SHOW = true;

    Menu::SetAutoHide(true);
}


void Menu::Hide()
{
    S_MENU_SHOW = false;

    Menu::SetAutoHide(true);
}


bool Menu::IsShown()
{
    return S_MENU_SHOW && GetMainPage() != nullptr;
}


void Menu::ClosePage(Page *page)
{
    page->OwnData()->funcOnOpenClose(false);

    Page *keeper = (Page *)page->Keeper();

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == GetMainPage())
    {
        Menu::Hide();
    }
}


void Menu::CloseIfSubPage(Page *parent, Page *page)
{
    if (page == parent)
    {
        ClosePage(parent);
    }

    if (page->IsSubPage(parent))
    {
        while (page)
        {
            ClosePage(page);
            page = (Page *)page->Keeper();
        }
    }
}


Item *Menu::OpenedItem()
{
    return LastOpened((Page *)GetMainPage());
}


/*
static void CloseDebugPages()
{
    const Page *mainPage = Menu::GetMainPage();

    Menu::SetMainPage(PageFunction::self);

    Menu::SetMainPage(PageService::self);

    PageService::EnablePageDebug();

    if (Menu::OpenedItem()->ExistKeeper(PageDebug::self))
    {
        while (Menu::OpenedPage() != PageService::self)
        {
            Menu::CloseOpenedItem();
        }
    }

    PageService::DisablePageDebug();

    Menu::SetMainPage(mainPage);
}
*/


void Menu::Init()
{
    PageFreqMeter::Init();
    PageDisplay::Settings::Colors::Init();

    CloseAllBadOpenedPages();
}


void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();

    if (item->Is(TypeItem::Page))
    {
        ClosePage((Page *)item); //-V1027
    }
    else
    {
        item->Open(false);
    }
}


Page *Menu::OpenedPage()
{
    Item *item = OpenedItem();

    return (item->IsPage()) ? (Page *)item : nullptr;
}

Item *Menu::LastOpened(Page *page)
{
    Item *result = &Item::empty;

    if (page)
    {
        if (page->CurrentItemIsOpened())
        {
            int8 posActItem = page->PosCurrentItem();
            Item *item = page->GetItem(posActItem);

            if (page->GetItem(posActItem)->Is(TypeItem::Page))
            {
                result = LastOpened((Page *)item);
            }
            else
            {
                result = item;
            }
        }
        else
        {
            result = page;
        }
    }

    return result;
}


Item *Menu::CurrentItem()
{
    Item *result = OpenedItem();

    int8 pos = ((Page *)result)->PosCurrentItem();

    if (result->Is(TypeItem::Page) && pos != 0x7f)
    {
        result = ((Page *)result)->GetItem(pos);
    }

    return result;
}


void Menu::DrawHintItem(int x, int y, int width)
{
    if (itemHint)
    {
        static pString names[TypeItem::Count] =
        {
            "",                   // Item_None
            "",                   // Item::Type::Choice
            "������",             // Item::Type::Button
            "��������",           // Item::Type::Page
            "���������",          // Item::Type::Governor
            "",                   // Item::Type::GovernorColor
            "������"              // Item::Type::DrawButton
        };

        Page *item = (Page *)itemHint;

        const int _SIZE_ = 100;
        char title[_SIZE_];
        std::snprintf(title, _SIZE_, "%s \"%s\"", names[itemHint->data->type], item->data->title);

        Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

        y = Text(item->data->hint).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

        if (item->Is(TypeItem::GraphButton))
        {
            ((GraphButton *)item)->DrawHints(x, y, width);    //-V1027
        }
    }
}


int Menu::Y0()
{
    return Display::HEIGHT - 48;
}


void Menu::Draw(int field)
{
    if (field != 4)
    {
        return;
    }

    Region(640, 150).Fill(0, 330, Color::GRAY_50);

    DFont::Set(DTypeFont::_8);

    if (Menu::IsShown())
    {
        Item *item = OpenedItem();

        if (item)
        {
            if (!item->Is(TypeItem::Page))
            {
                item = (Item *)item->Keeper();
            }

            if (item)
            {
                item->Draw(true);
            }
        }
    }

    DrawHint();
}


void Menu::DrawHint()
{
    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;

        //Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
        //       LANG_RU ? "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
        //       "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�." :
        //       "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
        //       "To disable this mode, press the button HELP and hold it for 0.5s.",
        //       Color::BACK, Color::FILL);

        Text("������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
            "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�."
        ).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);

        y += 49;
        if (stringForHint)
        {
            Text(stringForHint).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::WHITE);
        }
        else if (itemHint)
        {
            DrawHintItem(x, y, width);
        }
    }
}


void Menu::SetItemForHint(const Item *item)
{
    stringForHint = nullptr;
    itemHint = (Item *)item;
}


const Item *Menu::ItemUnderFunctionalKey(Key::E key)
{
    const Item *result = &Item::empty;

    if (Key(key).IsFunctional())
    {
        Item *item = Menu::OpenedItem();

        if (item == nullptr)
        {
        }
        else if (item->Is(TypeItem::Page))
        {
            result = ((Page *)item)->ItemForFuncKey(key);
        }
        else
        {
            const Page *parent = item->Keeper();
            if (parent && parent->ItemForFuncKey(key) == item && item->IsActive())
            {
                result = item;
            }
        }
    }

    return result;
}


void Menu::CloseAllBadOpenedPages()
{
    Page *opened = (Page *)LastOpened((Page *)PageFunction::self); //-V1027

    CloseIfSubPage((Page *)PageRecorder::self, opened);
    CloseIfSubPage((Page *)PageFreqMeter::self, opened);
    CloseIfSubPage((Page *)PageFFT::self, opened);
    CloseIfSubPage((Page *)PageMemory::self, opened);

    opened = (Page *)LastOpened((Page *)PageMemory::self);

    CloseIfSubPage((Page *)PageROM::self, opened);
    CloseIfSubPage((Page *)PageRAM::self, opened);
}


int8 &Menu::Position::ActItem(PageName::E name)
{
    return S_MENU_POS_ACT_ITEM(name);
}


Page *Menu::PageFromName(PageName::E name)
{
    for(int i = 0; true; i++)
    {
        if(pages[i] == nullptr)
        {
            break;
        }
        else if(pages[i]->GetName() == name)
        {
            return (Page *)pages[i];
        }
    }

    return nullptr;
}


void Menu::SetMainPage(const Page *page)
{
    S_MENU_MAIN_PAGE = page->GetName();
}


const Page *Menu::GetMainPage()
{
    return PageFromName(S_MENU_MAIN_PAGE);
}
