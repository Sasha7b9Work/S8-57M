#pragma once


#define TIME_WAIT   3000    // ����� ������ ��������


struct State
{
    enum E
    {
        Start,
        Mount,            // ������������ ������
        WrongFlash,       // ������ ����, �� ��������� ������
        RequestAction,    // ��� ������ - ���������� ��� ���
        NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
        Upgrade,          // ������� ��������
        Ok                // ���������� ������ ���������
    };
};


class FDrive
{
    friend class CPU;

public:
    static void Init();

    // ������� ������� ����������
    static void AttemptUpdate();

    static bool FileExist(pchar fileName);

    // ��������� ���� ��� ������. ���������� ������ �����
    static int OpenFileForRead(pchar fileName);

    // ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    static int ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();

    static void *GetHandleHCD();

    static void *GetHandleUSBH();

    static float PercentsUpdated();

    static State::E State();

    class LL_
    {
    public:
        static void InitHCD(void *phost);

        static void SetToggle(uint8 pipe, uint8 toggle);

        static uint8 GetToggle(uint8 pipe);
    };
};
