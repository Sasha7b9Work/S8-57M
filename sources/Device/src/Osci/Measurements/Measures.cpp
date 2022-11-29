#include "defines.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


struct StructMeasure
{
    const char *name;
    const char UGO;
    uint8      notUsed0;
    uint8      notUsed1;
    uint8      notUsed2;
};

#define DEF_STRUCT_MEASURE(name, ugo) {name, ugo, 0, 0, 0}

static const StructMeasure sMeas[TypeMeasure::Count] =
{
    DEF_STRUCT_MEASURE("",            '\x00'),
    DEF_STRUCT_MEASURE("U ����",      '\x20'),
    DEF_STRUCT_MEASURE("U ���",       '\x25'),
    DEF_STRUCT_MEASURE("U ���",       '\x2a'),
    DEF_STRUCT_MEASURE("U ���� ���",  '\x40'),
    DEF_STRUCT_MEASURE("U ��� ���",   '\x45'),
    DEF_STRUCT_MEASURE("U ���",       '\x4a'),
    DEF_STRUCT_MEASURE("U ��",        '\x60'),
    DEF_STRUCT_MEASURE("U ���",       '\x65'),
    DEF_STRUCT_MEASURE("������+",     '\x6a'),
    DEF_STRUCT_MEASURE("������-",     '\x80'),
    DEF_STRUCT_MEASURE("������",      '\x85'),
    DEF_STRUCT_MEASURE("�������",     '\x8a'),
    DEF_STRUCT_MEASURE("�� ������",   '\xa0'),
    DEF_STRUCT_MEASURE("�� �����",    '\xa5'),
    DEF_STRUCT_MEASURE("����+",       '\xaa'),
    DEF_STRUCT_MEASURE("����-",       '\xc0'),
    DEF_STRUCT_MEASURE("������+",     '\xc5'),
    DEF_STRUCT_MEASURE("������-",     '\xca'),
    DEF_STRUCT_MEASURE("��������\xa7",'\xe0'),
    DEF_STRUCT_MEASURE("��������\xa6",'\xe5'),
    DEF_STRUCT_MEASURE("����\xa7",    '\xe0'),
    DEF_STRUCT_MEASURE("����\xa6",    '\xe5')
};


String TypeMeasure::GetName(TypeMeasure::E type)
{
    return String(sMeas[type].name);
}


bool Measure::IsActive()
{
    if(AutoMeasurements::posActive >= TableMeasures::NumCols() * TableMeasures::NumRows())
    {
        AutoMeasurements::posActive = 0;
    }
    return (row * TableMeasures::NumCols() + col) == AutoMeasurements::posActive;
}


void Measure::SetActive(int row, int col)
{
    AutoMeasurements::posActive = static_cast<int8>(row * TableMeasures::NumCols() + col);
}


char Measure::GetChar(TypeMeasure::E measure)
{
    return sMeas[measure].UGO;
}


void Measure::ChangeActive(int delta)
{
    Measure measure = AutoMeasurements::GetActiveMeasure();

    int row = measure.row;
    int col = measure.col;

    col += Math::Sign(delta);

    if (col < 0)
    {
        col = TableMeasures::NumCols() - 1;
        row--;
        if (row < 0)
        {
            row = TableMeasures::NumRows() - 1;
        }
    }
    else if (col == TableMeasures::NumCols()) //-V2516
    {
        col = 0;
        row++;
        if (row >= TableMeasures::NumRows())
        {
            row = 0;
        }
    }

    SetActive(row, col);
}


String Measure::Name()
{
    TypeMeasure::E type = GetType();

    return (type > TypeMeasure::None && type < TypeMeasure::Count) ? String(sMeas[type].name) : String("");
}


TypeMeasure::E Measure::GetType() //-V2506
{
    if(t == TypeMeasure::Count)
    {
        return S_MEAS_INDICATED(row * TableMeasures::NumCols() + col);
    }

    return t;
}


void Measure::ShortPressOnSmallButonMarker()
{
    if(S_MEAS_INDICATED(AutoMeasurements::posActive) == S_MEAS_MARKED)
    {
        S_MEAS_MARKED = TypeMeasure::None;
    }
    else
    {
        S_MEAS_MARKED = S_MEAS_INDICATED(AutoMeasurements::posActive);
    }
}


void Measure::SetMarkerVoltage(Chan::E ch, int num, float value)
{
    AutoMeasurements::markerVoltage[ch][num] = static_cast<int>(value);
}


void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    AutoMeasurements::markerTime[ch][num] = value;
}
