#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"


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



struct Osci
{
    friend struct Randomizer;
    friend struct ContextTester;

    static void Init();

    static void Update();

    static void OnPressButtonStart();

    static void DeInit();

    // (���� button == true) - ��� ������ �������
    static void Start(bool button);

    static void Stop();

    static bool IsRunning();
    
    // ��� �������� � ������ ��������� ���������
    static void Restart();
    
    // ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    static void OnChangedLengthMemoryChannel();
   
    // ��������� �������� ��������� �������������
    //static void LoadHoldfOff();
    
    // ��� ������� ����� �������� ��� ��������� ������ �������
    static void ChangedTrigStartMode();
      
    // ������� ������ ������������� ��� ������������ �������
    static void ClearDataRand();
  
    static void ReadData();

    static uint16 ReadLastRecord(Chan::E ch);

    // ���������� ���� �����������
    static void ProcessFlagPred();

    // ��������� ����� �������
    static void RunAutoSearch();

    // ������� ������ � SCPI, ���� ��� ����������
    static void SendDataToSCPI();

    // ���������� �������� ������
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

private:

    // ������ ������ ������ � ������� data
    static bool ReadDataChannel(Chan::E ch, uint8 *data);

    static bool ReadDataChannelRand(uint8 *address, uint8 *data);

    static void SendDataToSCPI(Chan::E ch);

    // ����� �������� �����, ������� � �������� ����� ������ ������ �� �������. ���� addrRead == 0xffff, �� ����� ������� ����� �������
    static uint16 addrRead;
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
