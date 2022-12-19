// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageMeasures.h"



DEF_CHOICE_2( cIsShow,                                                                                                                             //--- ��������� - ������� - ���������� ---
    "����������",
    "�������� ��� �� �������� ��������� �� �����",
    "���",
    "��",
    S_MEAS_SHOW, &PageAutoMeasures::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_NumberChunnels()
{
    return S_MEAS_SHOW;
}


DEF_CHOICE_7( cNumber,                                                                                                                             //--- ��������� - ������� - ���������� ---
    "����������"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "1",
    "2",
    "1x5",
    "2x5",
    "3x5",
    "6x1",
    "6x2",
    S_MEAS_ON_DISPLAY, &PageAutoMeasures::self, IsActive_NumberChunnels, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cChannels,                                                                                                                               //--- ��������� - ������� - ������ ---
    "������",
    "�� ����� ������� �������� ���������",
    "1",
    "2",
    "1 � 2",
    S_MEAS_SOURCE, &PageAutoMeasures::self, IsActive_NumberChunnels, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_4( pAuto,                                                                                                                                                 //--- ��������� - ���� ---
    "�������",
    "������ � ���������� �������������� ���������",
    &cIsShow,
    &cNumber,
    &cChannels,
    PageAutoMeasures::Tune::self,
    PageName::AutoMeasures, &PageMeasures::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageAutoMeasures::self = static_cast<const Page *>(&pAuto);
