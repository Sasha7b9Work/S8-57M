// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


struct StructMeasure
{
    pchar       name;
    const uint8 UGO;
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
};

#define DEF_STRUCT_MEASURE(name, ugo) {name, ugo, 0, 0, 0}

static const StructMeasure s_meas[TypeMeasure::Count] =
{
    DEF_STRUCT_MEASURE("",             22 - 22),
    DEF_STRUCT_MEASURE("U макс",       32 - 22),
    DEF_STRUCT_MEASURE("U мин",        42 - 22),
    DEF_STRUCT_MEASURE("U пик",        52 - 22),
    DEF_STRUCT_MEASURE("U макс уст",   62 - 22),
    DEF_STRUCT_MEASURE("U мин уст",    72 - 22),
    DEF_STRUCT_MEASURE("U амп",        82 - 22),
    DEF_STRUCT_MEASURE("U ср",         92 - 22),
    DEF_STRUCT_MEASURE("U скз",        102 - 22),
    DEF_STRUCT_MEASURE("Выброс+",      112 - 22),
    DEF_STRUCT_MEASURE("Выброс-",      122 - 22), 
    DEF_STRUCT_MEASURE("Период",       132 - 22),
    DEF_STRUCT_MEASURE("Частота",      142 - 22),
    DEF_STRUCT_MEASURE("Вр нараст",    152 - 22),
    DEF_STRUCT_MEASURE("Вр спада",     162 - 22),
    DEF_STRUCT_MEASURE("Длит+",        172 - 22),
    DEF_STRUCT_MEASURE("Длит-",        182 - 22),
    DEF_STRUCT_MEASURE("Скважн+",      192 - 22),
    DEF_STRUCT_MEASURE("Скважн-",      202 - 22),
    DEF_STRUCT_MEASURE("Задержка\xa7", 212 - 22),
    DEF_STRUCT_MEASURE("Задержка\xa6", 222 - 22),
    DEF_STRUCT_MEASURE("Фаза\xa7",     212 - 22),
    DEF_STRUCT_MEASURE("Фаза\xa6",     222 - 22)
};


String TypeMeasure::GetName(TypeMeasure::E type)
{
    return String(s_meas[type].name);
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
    AutoMeasurements::posActive = (int8)(row * TableMeasures::NumCols() + col);
}


uint8 Measure::GetChar(TypeMeasure::E measure)
{
    return s_meas[measure].UGO;
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
    else if (col == TableMeasures::NumCols())
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

    return (type > TypeMeasure::None && type < TypeMeasure::Count) ? String(s_meas[type].name) : String("");
}


TypeMeasure::E Measure::GetType()
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
    AutoMeasurements::markerVoltage[ch][num] = (int)(value);
}


void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    AutoMeasurements::markerTime[ch][num] = value;
}
