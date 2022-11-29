#pragma once
#include "Tester/Tester.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTester.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"



#pragma pack(push, 1)
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();

    // ���������� � �������� ����������� �����
    uint CalcWriteCRC32();

    // ��������� �� ���������
    static const Settings defaultSettings;

    uint                    crc32;        // ����������� ����� ������ ��������� � ����������� � ��� �����������. ����������� ����� �������������� ��� ����, ������� ���� �� ������� ����� uint-������
    uint                    number;       // ������ ������ ��������� � ������

    SettingsMenu            menu;
    SettingsDisplay         disp;
    SettingsChannel         ch[Chan::Count];
    SettingsTrig            trig;
    SettingsTime            time;
    SettingsMemory          mem;
    SettingsCursorsMeasures curs;
    SettingsAutoMeasures    meas;
    SettingsMath            math;
    SettingsFFT             fft;
    SettingsFreqMeter       freq;
    SettingsTester          test;   // ��������� ������� �����������
    SettingsMultimeter      mult;
    SettingsRecorder        rec;
    SettingsService         serv;
    SettingsDebug           dbg;
    uint           notUsed[2];
    uint notDeleted;                // ��� ���������� ������� ������. ����� ��� ����������� ������� ����������� �����
};
#pragma pack(pop)

extern Settings set;
