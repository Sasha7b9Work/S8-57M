// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Symbols.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"


DEF_CHOICE_3(cChoice,
    "Курсор",
    "Выбор курсора",
    "Не выбран",
    "1",
    "2",
    S_REC_CURSOR, &PageRecorder::Show::Cursors::self, Choice::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Right()
{

}

static void Cursors_Draw_Right(int x, int y)
{
    SymbolUGO(SymbolUGO::TRIANGLE_RIGHT).Draw(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bRight,
    "Вправо",
    "Перемещение курсора вправо",
    &PageRecorder::Show::Cursors::self, Item::Active, OnPress_Right, Cursors_Draw_Right
)


static void OnPress_Left()
{

}

static void FFT_Draw_Left(int x, int y)
{
    SymbolUGO(SymbolUGO::TRIANGLE_LEFT).Draw(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bLeft,
    "Влево",
    "Перемещение курсора влево",
    &PageRecorder::Show::Cursors::self, Item::Active, OnPress_Left, FFT_Draw_Left
)


DEF_EMPTY_ITEM(e433, &PageRecorder::Show::Cursors::self)
DEF_EMPTY_ITEM(e533, &PageRecorder::Show::Cursors::self)


DEF_PAGE_5( pRecorderCursors,
    "КУРСОРЫ",
    "Работа курсорами",
    &cChoice,
    &bLeft,
    &bRight,
    &e433,
    &e533,
    PageName::Recorder_Show_Choice_Cursors,
    &PageRecorder::Show::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page *const PageRecorder::Show::Cursors::self = (const Page *)&pRecorderCursors;
