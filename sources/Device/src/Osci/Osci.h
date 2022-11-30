// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Hardware/HAL/HAL_PINS.h"


#define OSCI_IN_MODE_P2P        (S_TIME_BASE >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (S_TIME_BASE <= TBase::_50ns)



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

    uint16 ReadLastRecord(Chan::E ch);

    // ���������� ���� �����������
    void ProcessFlagPred();

    // ��������� ����� �������
    void RunAutoSearch();

    // ������� ������ � SCPI, ���� ��� ����������
    void SendDataToSCPI();

    // ���������� �������� ������
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };
};


class Roller
{
public:
    // ������� ����� �������� ����� ������ �������
    static void Prepare();

    // ��� ������� ����� ��������� �������� �� ����� ���������� ��������� ��� ������ ����� ����������� ������, ���� �� ��������� � ���������� ������
    static void ReadPoint();

    // ���������� true, ���� ����� �������� ���������� �����
    static bool NeedDraw();

    // ���������� ��������� �� ������, ������� ����� ��������
    static DataSettings *GetDS();

    // ��������� buffer ������� ��� ���������. width - ������ ���� ��� ���������. ���������� �������, � ������� ����� �������� �����������
    static int FillScreenBuffer(Chan::E ch, Buffer &buffer, int width);

private:

    // ������� ���������� ��������� �����
    static void (*addPoint)(BitSet16, BitSet16);
    static void AddPointPeakDetEnabled(BitSet16 dataA, BitSet16 dataB);
    static void AddPointPeakDetDisabled(BitSet16 dataA, BitSet16 dataB);

    // ���������� true, ���� ������ ����� ��������� �������
    static bool FirstDrawThisFrame();

    // ��������� �� ��������� ������� ������
    static DataSettings *ds;

    // ������� �����, ������� ����� �������� ���������
    static int currentPoint;

    // � ���� ����� ������� �������� ��������� �������� ����������� ������. ���� firstOnDisplay == -1, �� ����� ��������� ������� ����� � �������� ������ ���������
    static int firstOnDisplay;
};


// 
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
