// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/Recorder.h"


#define S_REC_ENABLED_A         (set.rec.enA)
#define S_REC_ENABLED_B         (set.rec.enB)
#define S_REC_ENABLED_SENSOR    (set.rec.enSensor)
#define S_REC_CURSOR            (set.rec.cursor)
#define S_REC_CURSOR_IS_1       (S_REC_CURSOR == DisplayRecorder::Cursor::_1)
#define S_REC_CURSOR_IS_2       (S_REC_CURSOR == DisplayRecorder::Cursor::_2)
#define S_REC_INFO_IS_SHOWN     (set.rec.showInfo == 0)


struct SettingsRecorder
{ //-V802
    bool                       enA;            // ��������� ������ ������ 1
    bool                       enB;            // ��������� ������ ������ 2
    bool                       enSensor;       // ��������� ������ ��������� �������
    Recorder::TypeMemory::E    typeMemory;     // ����� �������� ������������/������������ ������
    uint8                      showInfo;       // ������� ��� �����������
    Recorder::Axis::E          axisZoom;       // ������� ��� ���������������
    Recorder::ScaleX::E        scaleX;         // ������������� ������� �� �������
    DisplayRecorder::Cursor::E cursor;         // ������, ������� ������ ����� ���������
};


namespace PageRecorder
{
    extern const Page *const self;

    namespace Source
    {
        extern const Page *const self;
    };

    namespace Show
    {
        extern const Page *const self;

        namespace Choice
        {
            extern const Page *const self;
        };

        namespace Cursors
        {
            extern const Page *const self;
        };
    };
};
