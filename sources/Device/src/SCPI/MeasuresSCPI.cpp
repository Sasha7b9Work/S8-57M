// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :MEASURE:DISPLAY
static pchar FuncDisplay(pchar);
static void HintDisplay(String *);
static bool TestDisplay();

// :MEASURE:SOURCE
static pchar FuncSource(pchar);
static void HintSource(String *);
static bool TestSource();

// :MEASURE:VALUE
static pchar Meas_FuncValue(pchar);
static void Meas_HintValue(String *);
static bool Meas_TestValue();


const StructSCPI SCPI::measures[] =
{
    SCPI_LEAF(":DISPLAY", FuncDisplay,    TestDisplay,    "", HintDisplay),
    SCPI_LEAF(":SOURCE",  FuncSource,     TestSource,     "", HintSource),
    SCPI_LEAF(":VALUE",   Meas_FuncValue, Meas_TestValue, "", Meas_HintValue),
    SCPI_EMPTY()
};


static pString display[] =
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

    SCPI_PROCESS_ARRAY(display, DisplayMeasures(i));
}


static pString source[] =
{
    " 1",
    " 2",
    " BOTH",
    ""
};

static pchar FuncSource(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(source[S_MEAS_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_MEAS_SOURCE = static_cast<MeasuresSource::E>(i));
}


static pString values[] =
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

    SCPI_PROCESS_ARRAY(values, AutoMeasuresSender::DesignateForSending(static_cast<TypeMeasure::E>(i)));
}


static void HintDisplay(String *)
{

}


static void HintSource(String *)
{

}


static void Meas_HintValue(String *)
{

}


static bool TestDisplay()
{
    return false;
}


static bool TestSource()
{
    return false;
}


static bool Meas_TestValue()
{
    return false;
}
