// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :FREQMETER:AVEPERIODS
static pchar FuncAvePeriods(pchar);
static void HintAvePeriods(String *);
static bool TestAvePeriods();

// :FREQMETER:LABELS
static pchar FuncLabels(pchar);
static void HintLabels(String *);
static bool TestLabels();

// :FREQMETER:MEASURE
static pchar FuncMeasure(pchar);
static void HintMeasure(String *);
static bool TestMeasure();

// :FREQMETER:MODE
static pchar FuncMode(pchar);
static void HintMode(String *);
static bool TestMode();

// :FREQMETER:TIME
static pchar FuncTime(pchar);
static void HintTime(String *);
static bool TestTime();

// :FREQMETER:VALUE
static pchar FuncValue(pchar);
static void HintValue(String *);
static bool TestValue();


const StructSCPI SCPI::freqmeter[] =
{
    SCPI_LEAF(":AVEPERIODS", FuncAvePeriods, TestAvePeriods, "", HintAvePeriods),
    SCPI_LEAF(":LABELS",     FuncLabels,     TestLabels,     "", HintLabels),
    SCPI_LEAF(":MEASURE",    FuncMeasure,    TestMeasure,    "", HintMeasure),
    SCPI_LEAF(":MODE",       FuncMode,       TestMode,       "", HintMode),
    SCPI_LEAF(":TIME",       FuncTime,       TestTime,       "", HintTime),
    SCPI_LEAF(":VALUE?",     FuncValue,      TestValue,      "", HintValue),
    SCPI_EMPTY()
};


static pString periodsReverse[] =
{
    " 1E0",
    " 1E1",
    " 1E2",
    " 1E3",
    " 1E4",
    " 1E5",
    ""
};

static void SetNumberPeriods(int i)
{
    if (S_FREQ_MODE_MEASURE_IS_PERIOD)
    {
        S_FREQ_NUMBER_PERIODS = (FreqMeter::NumberPeriods::E)i;

        FreqMeter::LoadPeriodSettings();
    }
}

static pchar FuncAvePeriods(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(periodsReverse[S_FREQ_NUMBER_PERIODS]));

    SCPI_PROCESS_ARRAY(periodsReverse, SetNumberPeriods(i));
}


static pString labels[] =
{
    " 100KHZ",
    " 1MHZ",
    " 10MHZ",
    " 100MHZ",
    ""
};

static void SetLabels(int i)
{
    if (S_FREQ_MODE_MEASURE_IS_PERIOD)
    {
        S_FREQ_FREQ_CLC = (FreqMeter::FreqClc::E)i;
        
        FreqMeter::LoadPeriodSettings();
    }
}

static pchar FuncLabels(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(labels[S_FREQ_FREQ_CLC]));

    SCPI_PROCESS_ARRAY(labels, SetLabels(i));
}


static pString measures[] =
{
    " FREQUENCY",
    " PERIOD",
    ""
};

static void SetMeasure(int i)
{
    S_FREQ_MODE_MEASURE = (FreqMeter::ModeMeasure::E)i;
    PageFreqMeter::Init();
}

static pchar FuncMeasure(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(measures[S_FREQ_MODE_MEASURE]));

    SCPI_PROCESS_ARRAY(measures, SetMeasure(i));
}


static void SetMode(int i)
{
    S_FREQ_METER_ENABLED = (i == 1);
    FreqMeter::Init();
}


static pchar FuncMode(pchar buffer)
{
    static pString mode[] =
    {
        " OFF",
        " ON",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(S_FREQ_METER_ENABLED ? " ON" : " OFF"));

    SCPI_PROCESS_ARRAY(mode, SetMode(i));
}


static pString time[] =
{
    " 100MS",
    " 1S",
    " 10S",
    ""
};

static void SetTime(int i)
{
    if(S_FREQ_MODE_MEASURE_IS_FREQUENCY)
    {
        S_FREQ_TIME_COUNTING = (FreqMeter::TimeCounting::E)i;
    
        FreqMeter::LoadFreqSettings();
    }
}

static pchar FuncTime(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(time[S_FREQ_TIME_COUNTING]));

    SCPI_PROCESS_ARRAY(time, SetTime(i));
}


static pchar FuncValue(pchar buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::freqMeter = true;

    SCPI_EPILOG(buffer);
}


static void HintAvePeriods(String *)
{

}


static void HintLabels(String *)
{

}


static void HintMeasure(String *)
{

}


static void HintMode(String *)
{

}


static void HintValue(String *)
{

}


static void HintTime(String *)
{

}


static bool TestAvePeriods()
{
    return false;
}


static bool TestLabels()
{
    return false;
}


static bool TestMeasure()
{
    return false;
}


static bool TestMode()
{
    return false;
}


static bool TestTime()
{
    return false;
}


static bool TestValue()
{
    return false;
}
