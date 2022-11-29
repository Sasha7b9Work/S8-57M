#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"


static void Draw_Right(int x, int y)
{
    for (int i = 0; i < 2; i++)
    {
        Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + i * 12, y + 2);
    }
}

DEF_GRAPH_BUTTON( bScreenRight,                                                                                                     //--- ������� - ����������� - �������� - ����� ������ ---
    "����� ������",
    "����������� ���� ��������� �� ���� ����� ������",
    &PageRecorder::Show::self, Item::Active, DisplayRecorder::MoveWindowRight, Draw_Right
)


static void Draw_Left(int x, int y)
{
    for (int i = 0; i < 2; i++)
    {
        Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + i * 12, y + 2);
    }
}

DEF_GRAPH_BUTTON( bScreenLeft,                                                                                                       //--- ������� - ����������� - �������� - ����� ����� ---
    "����� �����",
    "����������� ���� ��������� �� ���� ����� �����",
    &PageRecorder::Show::self, Item::Active, DisplayRecorder::MoveWindowLeft, Draw_Left
)


DEF_CHOICE_3( cSpeed,
    "��������",
    "����� �������� ����������� ������",
    "1 ������",
    "1 �����",
    "10 �������",
    DisplayRecorder::speed, &PageRecorder::Show::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cCursor,
    "������",
    "����� �������",
    "1",
    "2",
    S_REC_CURSOR, &PageRecorder::Show::self, Choice::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cInfo,
    "����������",
    "",
    "����������",
    "������",
    set.rec._showInfo, &PageRecorder::Show::self, Choice::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_PageShow()
{
    return !Recorder::InRecordingMode();
}

static bool HandlerKey_PageShow(const KeyEvent &event) //-V2506
{
    if (event.IsPress() || event.IsRepeat())
    {
        if (event.IsArrowLeft())
        {
            DisplayRecorder::MoveCursorLeft();
            return true;
        }
        else if (event.IsArrowRight()) //-V2516
        {
            DisplayRecorder::MoveCursorRight();
            return true;
        }
    }

    return false;
}

static void OnOpenClose_PageShow(bool open)
{
    if (open)
    {
        HAL_BUS_CONFIGURE_TO_FSMC();
        DisplayRecorder::SetDisplayedRecord(StorageRecorder::LastRecord(), false);
    }
}


DEF_PAGE_5( pShow,                                                                                                                                 //--- ������� - ����������� - �������� --- //-V2567
    "��������",
    "�������� ���������� ������",
    &bScreenLeft,
    &bScreenRight,
    &cSpeed,
    &cCursor,
    &cInfo,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::NormalTitle, OnOpenClose_PageShow, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::Show::self = static_cast<const Page *>(&pShow);
