// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/Recorder.h"


#define S_REC_ENABLED_A         (set.rec._enA)
#define S_REC_ENABLED_B         (set.rec._enB)
#define S_REC_ENABLED_SENSOR    (set.rec._enSensor)
#define S_REC_CURSOR            (set.rec._cursor)
#define S_REC_CURSOR_IS_1       (S_REC_CURSOR == DisplayRecorder::Cursor::_1)
#define S_REC_CURSOR_IS_2       (S_REC_CURSOR == DisplayRecorder::Cursor::_2)
#define S_REC_INFO_IS_SHOWN     (set.rec._showInfo == 0)


struct SettingsRecorder
{ //-V802
    bool                       _enA;            // ��������� ������ ������ 1
    bool                       _enB;            // ��������� ������ ������ 2
    bool                       _enSensor;       // ��������� ������ ��������� �������
    Recorder::TypeMemory::E    _typeMemory;     // ����� �������� ������������/������������ ������
    uint8                      _showInfo;       // ������� ��� �����������
    Recorder::Axis::E          _axisZoom;       // ������� ��� ���������������
    Recorder::ScaleX::E        _scaleX;         // ������������� ������� �� �������
    DisplayRecorder::Cursor::E _cursor;         // ������, ������� ������ ����� ���������
};


struct PageRecorder
{
    static const Page *const self;

    struct Source
    {
        static const Page *const self;
    };

    struct Show
    {
        static const Page *const self;

        struct Choice
        {
            static const Page *const self;
        };

        struct Cursors
        {
            static const Page *const self;
        };
    };
};
