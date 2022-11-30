// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


#define S_SERV_ENUM_VOLUME          (set.serv._enumVolume)
#define S_SERV_SHOW_INFO_VOLTAGE    (set.serv._showInfoVoltage)
#define S_SERV_TIME_DISABLE_DISPLAY (set.serv._timeDisableDisplay)
#define S_SERV_TIME_DISABLE_POWER   (set.serv._timeDisablePower)


struct SettingsService
{
    Calibrator::Mode::E _calibratorMode;        // ����� ������ �����������.
    uint8               _enumVolume;            // ��������� �����
    bool                _showInfoVoltage;       // ���������� ���������� ������� �� ������
    int16               _timeDisableDisplay;    // ����� ���������� ������� � �������
    int16               _timeDisablePower;      // ����� ���������� ������� � �������
    uint8               SoundVolume();          // ���������� �������� �� 0 (����) �� 100 (����)
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    static void DecodePassword(const KeyEvent &event);
    static void EnablePageDebug();
    static void DisablePageDebug();

    struct Battery
    {
        static const Page *const self;
    };


    struct Calibrate
    {
        static const Page *const self;
    };


    struct Information
    {
        static const Page *const self;
    };
};


struct PageRTC
{
    static const Page *const self;
};
