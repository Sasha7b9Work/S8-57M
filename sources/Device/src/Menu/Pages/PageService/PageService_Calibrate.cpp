// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/Calibrator.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


static void OnChanged_Calibrator(bool)
{
    FPGA::LoadCalibratorMode();
}

DEF_CHOICE_2( cCalibrator, // -V206                                                                                                                //--- ������ - ���������� - ���������� ---
    "����������",
    "����� ������ �����������",
    "�����",
    "����",
    S_SERV_CALIBRATOR_MODE, &PageService::Calibrate::self, Item::Active, OnChanged_Calibrator, Choice::AfterDraw
)


static bool IsActive_Calibrate()
{
    return true;
}

static void OnPress_Calibrate()
{
    Calibrator::Calibrate();
}

DEF_BUTTON( bCalibrate,                                                                                                                           //--- ������ - ���������� - ����������� ---
    "�����������",
    "������ ��������� ����������",
    &PageService::Calibrate::self, IsActive_Calibrate, OnPress_Calibrate
)


DEF_PAGE_2( pCalibrate,                                                                                                                                         //--- ������ - ���������� ---
    "����������",
    "����������� ������������ � ���������� ������������",
    &cCalibrator,
    &bCalibrate,
    PageName::Service_Calibrator, &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::Calibrate::self = (const Page *)&pCalibrate;
