// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"


static void OnChanged_Gates(bool)
{
    Osci::ClearDataRand();
    setNRST.Save();
}


DEF_CHOICE_21( gGateMax,                                                                                                                           //--- ������� - ����-��� - ������ ���� ---
    "������ ����",
    "������������� ����������� ���������� �� ��������� ����� ��� �������������",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    "160",
    "170",
    "180",
    "190",
    "200",
    NRST_ENUM_GATE_MAX, &PageDebug::PageRand::self, Item::Active, OnChanged_Gates, Choice::AfterDraw
)



DEF_CHOICE_21( gGateMin,                                                                                                                            //--- ������� - ����-��� - ������ ��� ---
    "������ ���",
    "������������� ����������� ���������� �� �������� ����� ��� �������������",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    "160",
    "170",
    "180",
    "190",
    "200",
    NRST_ENUM_GATE_MIN, &PageDebug::PageRand::self, Item::Active, OnChanged_Gates, Choice::AfterDraw
)


DEF_CHOICE_4(cAverage,
    "�����������",
    "���������� ����������, ������� ����� ����������� ������ � �������������",
    DISABLE_RU,
    "2",
    "4",
    "8",
    NRST_ENUM_AVERAGE_RAND, &PageDebug::PageRand::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_3( pRand,                                                                                                                                               //--- ������� - ����-��� ---
    "����-���",
    "",
    &gGateMin,
    &gGateMax,
    &cAverage,
    PageName::Debug_Rand, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageRand::self = (const Page *)&pRand;
