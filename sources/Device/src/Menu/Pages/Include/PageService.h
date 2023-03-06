// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


#define S_SERV_ENUM_VOLUME          (set.serv.enumVolume)
#define S_SERV_SHOW_INFO_VOLTAGE    (set.serv.showInfoVoltage)
#define S_SERV_TIME_DISABLE_DISPLAY (set.serv.timeDisableDisplay)
#define S_SERV_TIME_DISABLE_POWER   (set.serv.timeDisablePower)


struct SettingsService
{
    Calibrator::Mode::E calibratorMode;         // ����� ������ �����������.
    uint8               enumVolume;             // ��������� �����
    bool                showInfoVoltage;        // ���������� ���������� ������� �� ������
    int16               timeDisableDisplay;     // ����� ���������� ������� � �������
    int16               timeDisablePower;       // ����� ���������� ������� � �������
    uint8               SoundVolume();          // ���������� �������� �� 0 (����) �� 100 (����)
};



namespace PageService
{
    extern const Page * const self;

    void OnPress_ResetSettings();

    void DecodePassword(const KeyEvent &event);
    void EnablePageDebug();
    void DisablePageDebug();

    namespace Battery
    {
        extern const Page *const self;
    };

    namespace Calibrate
    {
        extern const Page *const self;
    };

    namespace Information
    {
        extern const Page *const self;
    };
};


namespace PageRTC
{
    extern const Page *const self;
};
