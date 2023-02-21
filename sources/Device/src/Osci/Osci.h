// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Hardware/HAL/HAL_PINS.h"


#define OSCI_IN_MODE_P2P        (S_TIME_BASE >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (S_TIME_BASE <= TBase::MAX_RAND)



namespace OsciStateWork
{
    enum E
    {
        Stopped,        // ����������� � ��������� ��������
        Awaiting,       // ����������� � ��������� �������� �������������
        Triggered       // ����������������
    };

    OsciStateWork::E Current();
};



namespace Osci
{
    void Init();

    void Update();

    void OnPressButtonStart();

    void DeInit();

    // (���� button == true) - ��� ������ �������
    void Start(bool button);

    void Stop();

    bool IsRunning();

    // ��� �������� � ������ ��������� ���������
    void Restart();

    // ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    void OnChangedLengthMemoryChannel();

    // ��� ������� ����� �������� ��� ��������� ������ �������
    void ChangedTrigStartMode();

    // ������� ������ ������������� ��� ������������ �������
    void ClearDataRand();

    void ReadData();

    uint16 ReadLastRecord(Ch::E);

    // ���������� ���� �����������
    void ProcessFlagPred();

    // ��������� ����� �������
    void RunAutoSearch();

    // ������� ������ � SCPI, ���� ��� ����������
    void SendDataToSCPI();

    // ���������� �������� ������
    struct CircuitController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };
};


namespace Roller
{
    // ������� ����� �������� ����� ������ �������
    void Prepare();

    // ��� ������� ����� ��������� �������� �� ����� ���������� ��������� ��� ������ ����� ����������� ������, ���� �� ��������� � ���������� ������
    void ReadPoint();

    // ���������� true, ���� ����� �������� ���������� �����
    bool NeedDraw();

    // ���������� ��������� �� ������, ������� ����� ��������
    DataSettings *GetDS();

    // ��������� buffer ������� ��� ���������. width - ������ ���� ��� ���������. ���������� �������, � ������� ����� �������� �����������
    int FillScreenBuffer(Ch::E, Buffer &buffer, int width);
};


struct ShiftPoint
{
    enum E
    {
        FAIL,   // �������� ���������� �� �������, ����� ��������� �� ����
        READED  // ��������� ��������� �����
    } type;

    int shift;

    ShiftPoint(E t = FAIL, int s = 0) : type(t), shift(s)
    {
    }
};
