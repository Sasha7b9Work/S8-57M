// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


#define S_MENU_SHOW                     (set.menu.show)
#define S_MENU_POS_ACT_ITEM(name)       (set.menu.posActItem[name])
#define S_MENU_CURRENT_SUB_PAGE(name)   (set.menu.currentSubPage[name])
#define S_MENU_MAIN_PAGE                (set.menu.mainPage)


struct SettingsMenu
{
    bool        show;                               // ���� true, �� ����� ���������� ������� ��������
    int8        posActItem[PageName::Count];        // \brief ������� ��������� ������. bit7 == 1 - item is opened, 0x7f - ��� ��������� ������.
    int8        currentSubPage[PageName::Count];    // ����� ������� �����������.
    PageName::E mainPage;                           // ��� ������� ������� ��������
};



namespace Menu
{
    namespace Position
    {
        // ���������� ������� ��������� ������ �� �������� name
        int8 &ActItem(PageName::E name);
    };

    void Init();

    void ChangeStateFlashDrive();

    void Update();

    void Draw(int field);

    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    void SetAutoHide(bool active);

    void Show();

    void Hide();

    bool IsShown();

    Item *OpenedItem();

    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    Item *CurrentItem();

    Page *OpenedPage();

    // ���������� ��������� �� ������� ������� ��������
    const Page *GetMainPage();

    void CloseOpenedItem();

    void SetItemForHint(const Item *item);

    // � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    int Y0();

    const Item *ItemUnderFunctionalKey(Key::E key);

    // ���������� ��������� �� �������� � ������ name
    Page *PageFromName(PageName::E name);

    // ������������� ������� ������� ��������
    void SetMainPage(const Page *page);

    namespace Title
    {
        const int HEIGHT = 18;
        const int WIDTH = Display::WIDTH / 5 * 2 - 1;
    };
};
