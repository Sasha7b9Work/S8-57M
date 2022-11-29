#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"



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


DEF_CHOICE_3( cSource,                                                                                                                                  //--- ������� - ������ - �������� ---
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


static bool HandlerKey_FFT_Cursors(const KeyEvent &event) //-V2506
{
    if (event.IsArrow())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            int delta = (event.IsArrowUp() || event.IsArrowRight()) ? 1 : -1;

            S_FFT_POS_ACTIVE_CURSOR += static_cast<uint8>(delta);
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }
    else if(event.key == Key::F1 && event.IsRelease()) //-V2516
    {
        S_FFT_ACTIVE_CURSOR = static_cast<uint8>((S_FFT_ACTIVE_CURSOR + 1) % 2);
        return true;
    }

    return false;
}

static bool ShowTitle()
{
    return false;
}


static void OnCursors_BeforeDraw()
{
    Region(60, 20).DrawBounded(0, 219, Color::BACK, Color::FILL);

    Text(S_FFT_ACTIVE_CURSOR_IS_0 ? "������ 1" : "������ 2").Draw(12, 224, Color::FILL);
}


DEF_PAGE_0( pCursors,
    "�������",
    PageName::FFT_Cursors, &PageFFT::self, ShowTitle, Page::OpenClose, OnCursors_BeforeDraw, HandlerKey_FFT_Cursors
)


const Page * const PageFFT::Cursors::self = static_cast<const Page *>(&pCursors);

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

DEF_PAGE_6( pFFT,                                                                                                                                                   //--- ������ - ������ --- //-V2567
    "������",
    "����������� ������� �������� �������",
    &cView,
    &cScale,
    &cSource,
    &cWindow,
    &pCursors,
    &cRange,
    PageName::FFT, &PageFunction::self, IsActive_FFT, Page::NormalTitle, OnOpenClose_FFT, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFFT::self = static_cast<const Page *>(&pFFT);
