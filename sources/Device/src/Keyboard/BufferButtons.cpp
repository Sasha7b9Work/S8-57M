#include "defines.h"
#include "BufferButtons.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageService.h"


static const int SIZE = 100;

static KeyEvent buffer[SIZE];               // ����� �������� ��� �������
static KeyEvent prevPushEvent;              // ���������� �������� � ����� ������� ����������
static int end;                             // ������� ������� ���������� ����� � ������. ����� end == 0, ����� ������
static int start;                           // ������� ������� ��������� ������� � ������.
static uint timeLastControl = 0xFFFFFFFFU;  // ���������� �����

// ������������ �������� true ��������, ��� ��� ������� ��������� ������� �������������� - �� ����� ������������ ��� �������
static bool IsBeingProcessedPower(KeyEvent event);


void BufferButtons::Push(const KeyEvent &event)
{
    timeLastControl = TIME_MS;

    if (IsBeingProcessedPower(event))
    {
        return;
    }

    if ((event.key == prevPushEvent.key) &&             // ���� �������� ������, ������� ������ �������� "�������" �������,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // �� ��������� ��� ������� - ������ ��� ����������
    }

    PageMultimeter::DecodePassword(event);

    PageService::DecodePassword(event);

    // \todo �� ����� ������ ������ ��� ���������� ��������� ������ ��������. ������� �������� ��� �������� ������
    Beeper::Beep(event.type);

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


uint BufferButtons::TimeAfterControlMS()
{
    return TIME_MS - timeLastControl;
}


static bool IsBeingProcessedPower(KeyEvent event)
{
    static bool inProcessPowerOn = false;   // ������������� � true �������� ��������, ��� ��� ������� ��������� ������� ������� - ����� ��������� ���������� ������� � �� ����������
                                            // ���� ������ ��� ���������

    if (Display::Breaker::PowerOn())
    {
        inProcessPowerOn = true;
    }

    if (inProcessPowerOn)
    {
        if (event.IsRelease())
        {
            inProcessPowerOn = false;
        }
        return true;
    }

    return false;
}
