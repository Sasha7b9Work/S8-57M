// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <usbd_def.h>


namespace VCP
{
    static const int DEVICE_FS = 0;

    // �������������
    void Init();

    void SendDataAsynch(const uint8 *data, int size);

    void SendDataSynch(const void *data, int size = 0);

    // ��������� ������ ��� ������������ ����
    void SendStringAsynch(pchar data);

    // ��������� ������ ��� ������������ ����
    void SendStringSynch(char *data);

    // ��� ������ ��������� � ������������ ��������� \\r\\n
    void SendFormatStringAsynch(char *format, ...);

    // ��� ������ ��������� � ������������ ��������� \\r\\n
    void SendFormatStringSynch(char *format, ...);

    void SendByte(uint8 data);

    void Flush();

    extern bool connectedToUSB;

    extern bool cableUSBisConnected;

    extern void *handleUSBD;

    extern void *handlePCD;
};
