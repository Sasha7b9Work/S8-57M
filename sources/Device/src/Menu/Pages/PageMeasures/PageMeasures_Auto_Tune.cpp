// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives.h"
#include "common/Display/Font/Symbols.h"
#include "Menu/MenuItems.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


static void Draw_Markers(int x, int y)
{
    Char(SymbolUGO2::BIG_M, TypeFont::UGO, 4).Draw(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bMarkers,                                                                                                                          //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Маркер ---
    "Маркер",
    "Позволяет установить маркеры для визуального контроля измерений",
    &PageAutoMeasures::Tune::self, Item::Active, Measure::ShortPressOnSmallButonMarker, Draw_Markers
)


static void Draw_Settings(int x, int y)
{
    SymbolUGO(SymbolUGO::SETTINGS).Draw(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSettings,                                                                                                                      //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Настройка ---
    "Настройка",
    "Позволяет выбрать необходимые измерения",
    &PageAutoMeasures::Tune::self, Item::Active, PageChoiceMeasures::ChangeState, Draw_Settings
)


static bool IsActive_Tune() //-V524
{
    return S_MEAS_SHOW;
}

static bool HandlerKey_Tune(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        PageChoiceMeasures::OnKeyEvent(event);
        return true;
    }

    return false;
}


DEF_EMPTY_ITEM(e24, &PageAutoMeasures::Tune::self)
DEF_EMPTY_ITEM(e34, &PageAutoMeasures::Tune::self)
DEF_EMPTY_ITEM(e44, &PageAutoMeasures::Tune::self)
DEF_EMPTY_ITEM(e54, &PageAutoMeasures::Tune::self)


DEF_PAGE_5( pTune,  
    "НАСТРОИТЬ",
    "Переход в режим точной настройки количества и видов измерений",
    //&bMarkers,
    &bSettings,
    &e24,
    &e34,
    &e44,
    &e54,
    PageName::AutoMeasures_Tune, &PageAutoMeasures::self, IsActive_Tune, Page::NormalTitle, Page::OpenClose, HandlerKey_Tune
)

const Page *const PageAutoMeasures::Tune::self = (const Page *)&pTune;
