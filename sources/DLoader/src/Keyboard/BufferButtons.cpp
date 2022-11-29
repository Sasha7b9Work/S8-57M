#include "defines.h"
#include "BufferButtons.h"
#ifdef DEVICE
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif



static const int SIZE = 100;

/// ����� �������� ��� �������
static KeyEvent buffer[SIZE];
/// ���������� �������� � ����� ������� ����������
static KeyEvent prevPushEvent;
/// ������� ������� ���������� ����� � ������. ����� end == 0, ����� ������
static int end;
/// ������� ������� ��������� ������� � ������.
static int start;




void BufferButtons::Push(KeyEvent event) //-V2506
{
    if ((event.key == prevPushEvent.key) &&             // ���� �������� ������, ������� ������ �������� "�������" �������,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // �� ��������� ��� ������� - ������ ��� ����������
    }

    if(end == SIZE)
    {
        return;
    }

    buffer[end++] = event;

    prevPushEvent = event;
}


KeyEvent BufferButtons::Extract()
{
    KeyEvent result = buffer[start];

    start++;
    if(start == end)
    {
        start = 0;
        end = 0;
    }

    return result;
}


bool BufferButtons::IsEmpty()
{
    return end == 0;
}

