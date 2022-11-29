#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


static void OnPress_Exit()
{
    Menu::CloseOpenedItem();
}


DEF_BUTTON( bExit,
    "�����",
    "",
    &PageTester::self, Item::Active, OnPress_Exit
)


static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}

DEF_CHOICE_2( cPolarity,                                                                                                                                        //--- ������ - ���������� ---
    "����������",
    "���������� �������������� �����������",
    "+",
    "-",
    S_TEST_POLARITY, &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


DEF_CHOICE_2( cConductivity,                                                                                                                                  //--- ������ - ������������ ---
    "������������",
    "��� ������������ ����������� �����������",
    "n-p-n",
    "p-n-p",
    S_TEST_POLARITY, &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


DEF_CHOICE_2( cControl,                                                                                                                                         //--- ������ - ���������� ---
    "����������",
    "��� �������������� �����������",
    "����������",
    "���",
    S_TEST_CONTROL, &PageTester::self, Item::Active, PageTester::OnChanged_Control, Choice::AfterDraw
)


static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2( cStepU,                                                                                                                                        //--- ������-��������� - ��� ---
    "���",
    "��� �������������� ����������",
    "600 ��",
    "3 �",
    S_TEST_STEP_U, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
)


DEF_CHOICE_2( cStepI,
    "���",
    "��� ��������������� ����",
    "4 ���",
    "20 ���",
    S_TEST_STEP_I, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,
    "�����������",
    "",
    "������",
    "�����",
    S_TEST_VIEW_MODE, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_9( cAveraging,
    "����������",
    "����� ���������� ����������� ���������, �� ������� ����� ���������� �������� ���������. ��� ���� ��� ��������, ��� ����� ���������� ���������� ���������, �� ������ �����",
    "1",
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    S_TEST_ENUM_AVERAGE, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnOpenClose_Tester(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_6_VAR( pTester,  //-V2567
    "������",
    "",
    &cControl,
    &cStepU,
    &cConductivity,
    &cViewMode,
    &bExit,
    &cAveraging,
    PageName::Tester, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Tester, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTester::self = static_cast<const Page *>(&pTester);


void PageTester::OnChanged_Control(bool)
{
    Page *page = const_cast<Page *>(&pTester); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    if (S_TEST_CONTROL_IS_VOLTAGE)
    {
        items[1] = const_cast<Choice *>(&cStepU); //-V2563 //-V2567
        items[2] = const_cast<Choice *>(&cPolarity); //-V2563 //-V2567
    }
    else
    {
        items[1] = const_cast<Choice *>(&cStepI); //-V2563 //-V2567
        items[2] = const_cast<Choice *>(&cConductivity); //-V2563 //-V2567
    }

    Tester::LoadStep();
}
