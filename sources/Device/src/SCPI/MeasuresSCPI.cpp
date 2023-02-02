// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :MEASURE:DISPLAY
static pchar FuncDisplay(pchar);
static void HintDisplayM(String *);
static bool TestDisplayM();

// :MEASURE:SOURCE
static pchar FuncSource(pchar);
static void HintSourceM(String *);
static bool TestSourceM();

// :MEASURE:VALUE
static pchar Meas_FuncValue(pchar);
static void Meas_HintValue(String *);
static bool Meas_TestValue();


const StructSCPI SCPI::measures[] =
{
    SCPI_LEAF(":DISPLAY", FuncDisplay,    TestDisplayM,   "", HintDisplayM),
    SCPI_LEAF(":SOURCE",  FuncSource,     TestSourceM,    "", HintSourceM),
    SCPI_LEAF(":VALUE",   Meas_FuncValue, Meas_TestValue, "", Meas_HintValue),
    SCPI_EMPTY()
};


static pcharc displ[] =
{
    " OFF",
    " ON",
    ""
};

static void DisplayMeasures(int i)
{
    S_MEAS_SHOW = (i == 1);
}

static pchar FuncDisplay(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(S_MEAS_SHOW ? " ON" : " OFF"));

    SCPI_PROCESS_ARRAY(displ, DisplayMeasures(i));
}


static pchar FuncSource(pchar buffer)
{
    static pcharc source[] =
    {
        " 1",
        " 2",
        " BOTH",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(source[S_MEAS_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_MEAS_SOURCE = (MeasuresSource::E)i);
}


static pcharc values[] =
{
    " NNNNNNN",     // None,
    " VMAX",        // VoltageMax,
    " VMIN",        // VoltageMin,
    " VPP",         // VoltagePic,
    " VMAXEST",     // VoltageMaxSteady,
    " VMINEST",     // VoltageMinSteady,
    " VAMPL",       // VoltageAmpl,
    " VAVE",        // VoltageAverage,
    " VRMS",        // VoltageRMS,
    " VOVER+",      // VoltageVybrosPlus,
    " VOVER-",      // VoltageVybrosMinus,
    " PERIOD",      // Period,
    " FREQUENCY",   // Freq,
    " TIMERISE",    // TimeNarastaniya,
    " TIMEFALL",    // TimeSpada,
    " WIDTHP",      // DurationPlus,
    " WIDTHN",      // DurationMinus,
    " DUTY+",       // SkvaznostPlus,
    " DUTY-",       // SkvaznostMinus,
    " DELAY+",      // DelayPlus,
    " DELAY-",      // DelayMinus,
    " PHASE+",      // PhazaPlus,
    " PHASE-",      // PhazaMinus,
    ""
};

static pchar Meas_FuncValue(pchar buffer)
{
    pchar end = nullptr;

    end = SCPI::BeginWith(buffer, " VMAXEST");

    SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::VoltageMaxSteady));

    end = SCPI::BeginWith(buffer, " VMINEST");

    SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::VoltageMinSteady));

    end = SCPI::BeginWith(buffer, " TIMERISE");

    SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::TimeNarastaniya));

    SCPI_PROCESS_ARRAY(values, AutoMeasuresSender::DesignateForSending((TypeMeasure::E)i));
}


static void HintDisplayM(String *)
{

}


static void HintSourceM(String *)
{

}


static void Meas_HintValue(String *)
{

}


static bool TestDisplayM()
{
    return false;
}


static bool TestSourceM()
{
    return false;
}


static bool Meas_TestValue()
{
    return false;
}
