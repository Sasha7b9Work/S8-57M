#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Menu/MenuItems.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


static void Draw_Markers(int x, int y)
{
    Char(SymbolUGO2::BIG_M).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bMarkers,                                                                                                                          //--- ��������� - ��������� - ������ ---
    "������",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    &PageAutoMeasures::Tune::self, Item::Active, Measure::ShortPressOnSmallButonMarker, Draw_Markers
)


static void Draw_Settings(int x, int y)
{
    Char(SymbolUGO2::SETTINGS).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSettings,                                                                                                                      //--- ��������� - ��������� - ��������� ---
    "���������",
    "��������� ������� ����������� ���������",
    &PageAutoMeasures::Tune::self, Item::Active, PageChoiceMeasures::ChangeState, Draw_Settings
)


static bool IsActive_Tune()
{
    return S_MEAS_SHOW;
}

static bool HandlerKey_Tune(const KeyEvent &event) //-V2506
{
    if (event.IsArrow())
    {
        PageChoiceMeasures::OnKeyEvent(event);
        return true;
    }

    return false;
}

DEF_PAGE_1( pTune,   //-V2567
    "���������",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    //&bMarkers,
    &bSettings,
    PageName::AutoMeasures_Tune, &PageAutoMeasures::self, IsActive_Tune, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, HandlerKey_Tune
)

const Page *const PageAutoMeasures::Tune::self = static_cast<const Page *>(&pTune);
