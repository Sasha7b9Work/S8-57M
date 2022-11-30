// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <usbd_def.h>


class VCP
{
public:
    static const int DEVICE_FS = 0;

    // �������������
    static void Init();

    static void SendDataAsynch(const uint8 *data, int size);

    static void SendDataSynch(const void *data, int size = 0);
    
    // ��������� ������ ��� ������������ ����
    static void SendStringAsynch(const char *data);
    
    // ��������� ������ ��� ������������ ����
    static void SendStringSynch(char *data);
    
    // ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    
    // ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static bool connectedToUSB;

    static bool cableUSBisConnected;

    static void *handleUSBD;

    static void *handlePCD;
};
