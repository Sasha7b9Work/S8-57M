// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


static void OnCursors_Draw(int field);


static bool IsActive_Parameter()
{
    return S_FFT_ENABLED;
}


DEF_CHOICE_2( cView,                                                                                                                                 //--- ������� - ������ - ����������� ---
    "�����������",
    "�������� � ��������� ����������� �������",
    DISABLE_RU,
    ENABLE_RU,
    S_FFT_ENABLED, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cScale,                                                                                                                                      //--- ������� - ������ - ����� ---
    "�����",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "��������",
    "��������",
    S_FFT_SCALE, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cSourceFFT,                                                                                                                                  //--- ������� - ������ - �������� ---
    "��������",
    "����� ��������� ��� ������� �������",
    "����� 1",
    "����� 2",
    "����� 1 + 2",
    S_FFT_SOURCE, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ������� - ������ - ���� ---
    "����",
    "����� ���� ��� ������� �������",
    "�����������",
    "��������",
    "��������",
    "�����",
    S_FFT_WINDOW, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Range()
{
    return (S_FFT_ENABLED && S_FFT_SCALE_IS_LOG);
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ������� - ������ - �������� ---
    "��������",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "-40��",
    "-60��",
    "-80��",
    S_FFT_MAX_DB, &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
)


static bool HandlerKey_FFT_Cursors(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            int delta = (event.IsArrowUp() || event.IsArrowRight()) ? 1 : -1;

            S_FFT_POS_ACTIVE_CURSOR += (uint8)(delta);
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }
    else if((event.key == Key::F1 || event.key == Key::F2 || event.key == Key::F3 || event.key == Key::F4 || event.key == Key::F5) && event.IsRelease())
    {
        S_FFT_ACTIVE_CURSOR = (uint8)((S_FFT_ACTIVE_CURSOR + 1) % 2);
        return true;
    }

    return false;
}

static bool ShowTitle()
{
    return false;
}


static void OnOpenClose_Cursors(bool open)
{
    if (open)
    {
        Display::AdditionalFunctionDraw::Set(OnCursors_Draw);
    }
    else
    {
        Display::AdditionalFunctionDraw::Remove();
    }
}


DEF_PAGE_0( pCursors,
    "�������",
    PageName::FFT_Cursors, &PageFFT::self, ShowTitle, OnOpenClose_Cursors, HandlerKey_FFT_Cursors
)


static void OnCursors_Draw(int field)
{
    if (field == 4 && pCursors.IsCurrentItem())
    {
        int y = Display::HEIGHT - Item::Height() - 1;

        Region(Item::empty.Width(), Item::Height()).FillBounded(0, y, Color::BACK, Color::FILL);

        Text(S_FFT_ACTIVE_CURSOR_IS_0 ? "������ 1" : "������ 2").Draw(28, y + 15, Color::FILL);
    }
}


const Page * const PageFFT::Cursors::self = (const Page *)&pCursors;

static bool IsActive_FFT()
{
    return S_MATH_MODE_DRAW_IS_DISABLED;
}

static void OnOpenClose_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning("��������� �������������� �������");
    }
}


DEF_EMPTY_ITEM(c7, &PageFFT::self)
DEF_EMPTY_ITEM(c8, &PageFFT::self)
DEF_EMPTY_ITEM(c9, &PageFFT::self)
DEF_EMPTY_ITEM(c10, &PageFFT::self)


DEF_PAGE_10( pFFT,                                                                                                                                                   //--- ������ - ������ ---
    "������",
    "����������� ������� �������� �������",
    &cView,
    &cScale,
    &cSourceFFT,
    &cWindow,
    &pCursors,
    &cRange,
    &c7,
    &c8,
    &c9,
    &c10,
    PageName::FFT, &PageFunction::self, IsActive_FFT, Page::NormalTitle, OnOpenClose_FFT, Page::HandlerKeyEvent
)

const Page * const PageFFT::self = (const Page *)&pFFT;
