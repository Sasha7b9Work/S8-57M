// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Math/OsciMath.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "common/Command.h"


void Resolution::Set(E v)
{
    value = v;;

    HAL_BUS::Panel::Send(Command::ResolutionDisplay, (uint8)value);
}


void ENumAverage::Set(ENumAverage::E v)
{
    S_OSCI_ENUM_AVERAGE = v;
    AveragerOsci::SettingChanged();
}


pString ENumAccum::ToString(ENumAccum::E v)
{
    static const pString strings[ENumAccum::Count] =
    {
        "1",
        "2",
        "4",
        "8",
        "16",
        "32",
        "64",
        "128",
        "INF"
    };

    return strings[v];
}


DEF_CHOICE_10( cSmoothing,                                                                                                                                    //--- ������� - ����������� ---
    "�����.",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    DISABLE_RU,
    "2 �����",
    "3 �����",
    "4 �����",
    "5 �����",
    "6 �����",
    "7 �����",
    "8 �����",
    "9 �����",
    "10 �����",
    S_DISP_ENUM_SMOOTH, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_5( cRefreshFPS,                                                                                                                                 //--- ������� - ������� ������ ---
    "������� ������",
    "����� ������������ ����� ��������� � ������� ������.",
    "25",
    "10",
    "5",
    "2",
    "1",
    S_DISP_ENUM_FPS, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cType, // -V206                                                                                                                                       //--- ������� - ����� ---
    "�����",
    "����� ���� �����",
    "��� 1",
    "��� 2",
    "��� 3",
    "��� 4",
    S_DISP_TYPE_GRID, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageDisplay::SetTypeGrid(TypeGrid::E v)
{
    S_DISP_TYPE_GRID = v;
}


static void OnChange_AverageNum(bool)
{
    AveragerOsci::SettingChanged();
}

DEF_CHOICE_9( cAverage_Num,                                                                                                                       //--- ������� - ���������� - ���������� ---
    "����������",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
    DISABLE_RU,
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    S_OSCI_ENUM_AVERAGE, &PageDisplay::self, Item::Active, OnChange_AverageNum, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,                                                                                                                                      //--- ������� - ����������� ---
    "�������",
    "����� ����� ����������� �������",
    "������",
    "�����",
    S_DISP_MAPPING, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_7( pDisplay,                                                                                                                                                       //--- ������� ---
    "�������",
    "�������� ��������� ����������� �������.",
    &cViewMode,
    PageDisplay::Accumulation::self,
    &cAverage_Num,
    &cSmoothing,
    &cType,
    &cRefreshFPS,
    PageDisplay::Settings::self,
    PageName::Display, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::self = static_cast<const Page *>(&pDisplay);
