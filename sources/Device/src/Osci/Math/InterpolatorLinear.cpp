// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Timer.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Math/OsciMath.h"


namespace InterpolatorLinear
{
    static uint8 *begin = nullptr;  // ������ �������������� ������
    static uint8 *end = nullptr;    // ����� �������������� ������


    // ��������� ��������� �������
    struct Segment
    {
        uint8 *start;   // ��������� �� ������ ������� �������
        uint8 *end;     // ��������� �� ������ ������� �� ��������
    };


    // ��������������� ������ ������ ������
    static void InterpolateChannel(uint8 *data, int size);

    // ������� ������ ������ ������� (�.�. � ������� ��������� ������� "������" ��������), ������� � �������� � ������� start
    static bool FindEmptySegment(uint8 *start, Segment *segment);

    // ��������������� ������ � ��������
    static void InterpolateSegment(Segment *segment);

    // ������� ����� ������� ��������. ���������� end, ���� ������ �������� ���
    static uint8 *FindEmptyElement(uint8 *const start);

    // ����� ������ "��������" ������� ������. ���������� end, ���� ������ �������� ���
    static uint8 *FindReadedElement(uint8 *const start);
}


bool InterpolatorLinear::FindEmptySegment(uint8 *start, Segment *segment)
{
    segment->start = FindEmptyElement(start);

    segment->end = FindReadedElement(segment->start);

    return (segment->start != end) && (segment->end != end);
}


uint8 *InterpolatorLinear::FindEmptyElement(uint8 * const start)
{
    uint8 *element = start;

    uint8 *interpol = IntRAM::DataRand(ChanA) + (start - begin);

    while(element != end)
    {
        if(*interpol == VALUE::NONE || *element == VALUE::NONE)
        {
            break;
        }
        element++;
        interpol++;
    }

    return element;
}


uint8 *InterpolatorLinear::FindReadedElement(uint8 * const start)
{
    uint8 *element = start;
    uint8 *interpol = IntRAM::DataRand(ChanA) + (start - begin);
    
    while(element != end)
    {
        if(*element != VALUE::NONE && *interpol != VALUE::NONE)
        {
            break;
        }
        element++;
        interpol++;
    }

    return element;
}


void InterpolatorLinear::InterpolateSegment(Segment *segment)
{
    if((segment->start == end) ||
       (segment->start == begin) ||
       (segment->end == end))
    {
        return;
    }

    float before = *(segment->start - 1);               // �������� ����� "������" ���������
    float after = *(segment->end);                      // �������� ����� "�������" ��������

    int numTicks = segment->end - segment->start + 1;   // �� ������� �������� ������ ��� �������

    float delta = (after - before) / numTicks;

    for(int tick = 0; tick < numTicks - 1; tick++)
    {
        float value = before + delta * (tick + 1);

        *(segment->start + tick) = (uint8)(value + 0.5F);
    }
}


void InterpolatorLinear::Run(DataSettings *ds)
{
    if(ds->en_a)
    {
        InterpolateChannel(ds->ch_a, ds->BytesInChannel());
    }
    if(ds->en_b)
    {
        InterpolateChannel(ds->ch_b, ds->BytesInChannel());
    }
}


void InterpolatorLinear::InterpolateChannel(uint8 *data, int size)
{
    int readed = 0;                                     // ����� ������� ��������� �����
    uint8 *pointer = data;
    uint8 *last = pointer + size;
    int halfSize = size / 2;

    while(pointer < last)
    {
        if(*pointer++ != VALUE::NONE)
        {
            readed++;
        }

        if(readed > halfSize)
        {
            begin = data;
            end = begin + size;

            Segment segment;

            segment.end = begin;

            while(FindEmptySegment(segment.end, &segment))
            {
                InterpolateSegment(&segment);
            }

            break;
        }
    }
}
