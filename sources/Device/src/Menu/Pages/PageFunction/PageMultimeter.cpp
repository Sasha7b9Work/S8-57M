#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#include "Settings/SettingsNRST.h"


static bool IsActive_Ranges()
{
    return S_MULT_AVP_DISABLED;
}


static void OnPress_Exit()
{
    Menu::CloseOpenedItem();
}


DEF_BUTTON( bExit,
    "�����",
    "",
    &PageMultimeter::self, Item::Active, OnPress_Exit
)


static void OnChange_VoltageDC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageDC,                                                                                                                                 //--- ���������� - ������ ---
    "��������",
    "�������� ���������",
    "2 �",
    "20 �",
    "500 �",
    S_MULT_RANGE_VOLTAGE_DC, &PageMultimeter::self, IsActive_Ranges, OnChange_VoltageDC, Choice::AfterDraw
)


static void OnChange_VoltageAC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageAC,                                                                                                                                 //--- ���������� - ������ ---
    "��������",
    "�������� ���������",
    "2 �",
    "20 �",
    "400 �",
    S_MULT_RANGE_VOLTAGE_AC, &PageMultimeter::self, IsActive_Ranges, OnChange_VoltageAC, Choice::AfterDraw
)



static void OnChange_CurrentDC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentDC,
    "��������",
    "�������� ���������",
    "20��",
    "2�",
    S_MULT_RANGE_CURRENT_DC, &PageMultimeter::self, IsActive_Ranges, OnChange_CurrentDC, Choice::AfterDraw
)


static void OnChange_CurrentAC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentAC,
    "��������",
    "�������� ���������",
    "20��",
    "2�",
    S_MULT_RANGE_CURRENT_AC, &PageMultimeter::self, IsActive_Ranges, OnChange_CurrentAC, Choice::AfterDraw
)


static void OnChange_Resistance(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_4( cRangesResistance,                                                                                                                                //--- ���������� - ������ ---
    "��������",
    "�������� ���������",
    "2 ���",
    "20 ���",
    "200 ���",
    "10 ���",
    S_MULT_RANGE_RESISTANCE, &PageMultimeter::self, IsActive_Ranges, OnChange_Resistance, Choice::AfterDraw
)

DEF_CHOICE_7( cMode,
    "�����"
    ,
    "����� ���� ����������\n"
    "U= - ���������� ����������\n"
    "U~ - ���������� ����������\n"
    "I= - ���������� ���\n"
    "I~ - ���������� ���\n"
    "R - ������������� ����������� ����\n"
    "VD - ���� ������\n"
    "BELL - ���������"
    ,
    "U=",
    "U~",
    "I=",
    "I~",
    "R",
    "VD",
    "���������",
    S_MULT_MEASURE, &PageMultimeter::self, Item::Active, PageMultimeter::OnChanged_Mode, Choice::AfterDraw
)


static bool IsActive_Zero() //-V524
{
    return S_MULT_AVP_DISABLED;
}

static int8 zero = 0;

static void OnChanged_Zero(bool active)
{
    if(active)
    {
        Multimeter::LoadZero(zero);
    }
}

void PageMultimeter::EnableZero(bool enable)
{
    zero = enable ? 1 : 0;
    OnChanged_Zero(true);
}

DEF_CHOICE_2(cZero,
    "����",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, &PageMultimeter::self, IsActive_Zero, OnChanged_Zero, Choice::AfterDraw
)

bool PageMultimeter::ZeroEnabled()
{
    return (zero != 0);
}


static bool IsActive_AVP()
{
    return (zero == 0);
}

static void OnChanged_AVP(bool active)
{
    if(active)
    {
        Multimeter::ChangeAVP();
    }
}

DEF_CHOICE_2 ( cAVP,
    "���",
    "��������� �������",
    DISABLE_RU,
    ENABLE_RU,
    S_MULT_MODE_AVP, &PageMultimeter::self, IsActive_AVP, OnChanged_AVP, Choice::AfterDraw
)


DEF_CHOICE_2 (cShow,
    "����������",
    "",
    "� ������",
    "������",
    S_MULT_SHOW_ALWAYS, &PageMultimeter::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = const_cast<Page *>(PageMultimeter::self); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    items[1] = const_cast<Choice *>(&cRangesVoltageDC); //-V2563 //-V2567
    items[2] = const_cast<Choice *>(&cAVP); //-V2563 //-V2567
    items[3] = const_cast<Choice *>(&cZero); //-V2563 //-V2567
    items[4] = const_cast<Button* >(&bExit); //-V2563 //-V2567

    switch(S_MULT_MEASURE)
    {
    case MultimeterMeasure::VoltageDC:      items[1] = const_cast<Choice *>(&cRangesVoltageDC);       break; //-V2563 //-V2567
    case MultimeterMeasure::VoltageAC:      items[1] = const_cast<Choice *>(&cRangesVoltageAC);       break; //-V2563 //-V2567
    case MultimeterMeasure::CurrentDC:      items[1] = const_cast<Choice *>(&cRangesCurrentDC);       break; //-V2563 //-V2567
    case MultimeterMeasure::CurrentAC:      items[1] = const_cast<Choice *>(&cRangesCurrentAC);       break; //-V2563 //-V2567
    case MultimeterMeasure::Resistance:     items[1] = const_cast<Choice *>(&cRangesResistance);      break; //-V2563 //-V2567
    case MultimeterMeasure::TestDiode:  
    case MultimeterMeasure::Bell:           items[1] = &Item::empty; //-V2563
                                            items[2] = &Item::empty; //-V2563
                                            items[3] = &Item::empty;                                  break; //-V2563

    case MultimeterMeasure::Count:
        break;
    }

    Multimeter::ChangeMode();
}

static void OnOpenClose_Multimeter(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_7_VAR( pMultimeter, //-V2567
    "����������",
    "���������� �������� � ������ �����������",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &bExit,
    &cShow,
    &Item::empty,
    PageName::Multimeter, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Multimeter, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::self = static_cast<const Page *>(&pMultimeter);


static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
    setNRST.Save();
}

DEF_BUTTON( bCalibrate0,
    "������ 0",
    "",
    &PageMultimeter::Calibration::self, Item::Active, OnPress_Calibrate0
)


static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
    setNRST.Save();
}

DEF_BUTTON( bCalibrate1,
    "������ 1",
    "",
    &PageMultimeter::Calibration::self, Item::Active, OnPress_Calibrate1
)

DEF_PAGE_2( pCalibration, //-V2567
    "����������",
    "���������� �����������",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Multimeter_Cal, &PageMultimeter::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::Calibration::self = static_cast<const Page *>(&pCalibration);


void PageMultimeter::DecodePassword(const KeyEvent &event) //-V2506
{
#define NUM_SYMBOLS 10
    // ������
    static const Key::E password[NUM_SYMBOLS] =
    {
        Key::Function, Key::Function,
        Key::Measure,  Key::Measure,
        Key::Memory,   Key::Memory,
        Key::Display,  Key::Display,
        Key::Service,  Key::Service
    };
    // ����� ��������� ��������
    static int charsMatch = 0;


    if (!Device::InModeMultimeter())         // ������������� ���������� ������ � ������ �����������
    {
        charsMatch = 0;
        return;
    }

    if (Menu::OpenedItem() != const_cast<Choice *>(&cMode))       // � ����������� ��� ��������� ���� "�����" //-V2567
    {
        charsMatch = 0;
        return;
    }

    if (event.type != TypePress::Release)
    {
        return;
    }

    if (password[charsMatch++] == event.key)
    {
        if (charsMatch == NUM_SYMBOLS)
        {
            EnablePageCalibrate();

            Menu::CloseOpenedItem();

            DISPLAY_SHOW_WARNING("������ � ���������� ������");
        }
    }
    else
    {
        charsMatch = 0;
    }
}


void PageMultimeter::EnablePageCalibrate()
{
    Page *page = const_cast<Page *>(&pMultimeter); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    items[6] = const_cast<Page *>(PageMultimeter::Calibration::self); //-V2563 //-V2567
}


void PageMultimeter::DisablePageCalibrate()
{
    Page *page = const_cast<Page *>(&pMultimeter); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    items[6] = &Item::empty; //-V2563
}
