// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


// ¬иды измерений
struct TypeMeasure
{
    // »ндицируемые измерени€
#define S_MEAS_INDICATED(num)   (set.meas._measures[num])
#define S_MEAS_MARKED           (set.meas._marked)

    enum E
    {
        None,
        VoltageMax,
        VoltageMin,
        VoltagePic,
        VoltageMaxSteady,
        VoltageMinSteady,
        VoltageAmpl,
        VoltageAverage,
        VoltageRMS,
        VoltageVybrosPlus,
        VoltageVybrosMinus,
        Period,
        Freq,
        TimeNarastaniya,
        TimeSpada,
        DurationPlus,
        DurationMinus,
        SkvaznostPlus,
        SkvaznostMinus,
        DelayPlus,
        DelayMinus,
        PhazaPlus,
        PhazaMinus,
        Count
    };

    static String GetName(TypeMeasure::E type);
};


struct Measure
{
public:
    Measure(int r, int c) : row(r), col(c), t(TypeMeasure::Count) {};

    Measure(TypeMeasure::E type) : row(0), col(0), t(type) {};

    TypeMeasure::E GetType();

    String GetStringMeasure(Ch::E, char *buffer, int lenBuf);

    // ”станавливает активным следующее или предыдущее измерение
    static void ChangeActive(int delta);

    // –ассчитать позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT
    static float CalculateCursorU(Ch::E, float posCurT);

    // –ассчитать позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU
    static float CalculateCursorT(Ch::E, float posCurU, int numCur);

    static void SetMarkerVoltage(Ch::E, int num, float value);

    static void SetMarkerTime(Ch::E, int num, int value);

    static uint8 GetChar(TypeMeasure::E measure);

    static void ShortPressOnSmallButonMarker();

    // ¬озвращает true, если измерение активное - выбрано ручкой
    bool IsActive();

    String Name();

private:

    // —делать активным
    static void SetActive(int row, int col);

    int row;            // —трока в таблице, в которой находитс€ данное измерение
    int col;            //  олонка в таблице, в которой находитс€ данное измерение

    TypeMeasure::E t;
};
