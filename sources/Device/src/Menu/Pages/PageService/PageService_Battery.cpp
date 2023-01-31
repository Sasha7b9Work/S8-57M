// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cShow,                                                                                                                                  //--- ������ - ������� - ���������� ---  
    "����������",
    "��������/��������� ����� ����������� � ���������� ������� �� �����",
    DISABLE_RU,
    ENABLE_RU,
    S_SERV_SHOW_INFO_VOLTAGE, &PageService::Battery::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_1( pBattery,                                                                                                                                              //--- ������ - ������� ---
    "�������",
    "������� ������ � ��������������",
    &cShow,
    PageName::Service_Battery, &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageService::Battery::self = (const Page *)&pBattery;
