// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <cstring>


// :DISPLAY:ACCUMULATION
static pchar FuncAccumulation(pchar);
static void HintAccumulation(String *);
static bool TestAccumulation();

// :DISPLAY:AVERAGES
static pchar FuncAverages(pchar);
static void HintAverages(String *);
static bool TestAverages();

// :DISPLAY:BRIGHTNESS
static pchar FuncBrightness(pchar);
static void HintBrightness(String *);
static bool TestBrightness();

// :DISPLAY:FPS
static pchar FuncFPS(pchar);
static void HintFPS(String *);
static bool TestFPS();

// :DISPLAY:GRID
static pchar FuncGrid(pchar);
static void HintGrid(String *);
static bool TestGrid();

// :DISPLAY:MAPPING
static pchar FuncMapping(pchar);
static void HintMapping(String *);
static bool TestMapping();

// :DISPLAY:SMOOTHING
static pchar FuncSmoothing(pchar);
static void HintSmoothing(String *);
static bool TestSmoothing();


static pchar const grids[] =
{
    " TYPE1",
    " TYPE2",
    " TYPE3",
    " TYPE4",
    ""
};


static pcharc mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":ACCUMULATION", FuncAccumulation, TestAccumulation, "",                       HintAccumulation),
    SCPI_LEAF(":AVERAGE",      FuncAverages,     TestAverages,     "Number of averages",     HintAverages),
    SCPI_LEAF(":BRIGHTNESS",   FuncBrightness,   TestBrightness,   "",                       HintBrightness),
    SCPI_LEAF(":FPS",          FuncFPS,          TestFPS,          "",                       HintFPS),
    SCPI_LEAF(":GRID",         FuncGrid,         TestGrid,         "Grid type selection",    HintGrid),
    SCPI_LEAF(":MAPPING",      FuncMapping,      TestMapping,      "Signal display control", HintMapping),
    SCPI_LEAF(":SMOOTHING",    FuncSmoothing,    TestSmoothing,    "",                       HintSmoothing),
    SCPI_EMPTY()
};


static pcharc accumulationDirect[] =
{
    " OFF",
    " 2",
    " 4",
    " 8",
    " 16",
    " 32",
    " 64",
    " 128",
    " INFINITY",
    ""
};


static pchar FuncAccumulation(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(accumulationDirect[S_DISP_ENUM_ACCUM]));

    SCPI_IF_BEGIN_WITH_THEN(" CLEAR", PageDisplay::Accumulation::OnPress_Accumulation_Clear());

    SCPI_PROCESS_ARRAY(accumulationDirect, S_DISP_ENUM_ACCUM = (ENumAccum::E)i);
}


static void SetAverage(int i)
{
    static const ENumAverage::E _ave_[] =
    {
        ENumAverage::_128,
        ENumAverage::_16,
        ENumAverage::_256,
        ENumAverage::_8,
        ENumAverage::_2,
        ENumAverage::_32,
        ENumAverage::_64,
        ENumAverage::_1,
        ENumAverage::_4
    };

    ENumAverage::Set(_ave_[i]);
}

static pcharc averagesDirect[] = { " 1", " 2", " 4", " 8", " 16", " 32", " 64", " 128", " 256", "" };

static pchar FuncAverages(pchar buffer)
{
    static pcharc averages[] = { " 128", " 16", " 256", " 8", " 2", " 32", " 64", " 1", " 4", "" };

    SCPI_REQUEST(SCPI::SendAnswer(averagesDirect[S_OSCI_ENUM_AVERAGE]));

    SCPI_PROCESS_ARRAY(averages, SetAverage(i));
}


static pchar FuncBrightness(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(String("%d", set.disp.brightness).c_str()));

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        PageDisplay::SetBrightness(value);
        return end_str + 1;
    }

    return nullptr;
}


static pcharc fps[] =
{
    " 25",
    " 10",
    " 5",
    " 2",
    " 1",
    ""
};

static pchar FuncFPS(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(fps[S_DISP_ENUM_FPS]));

    SCPI_PROCESS_ARRAY(fps, S_DISP_ENUM_FPS = (ENumSignalsInSec::E)i);
}


static pchar FuncGrid(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(grids[S_DISP_TYPE_GRID]));

    SCPI_PROCESS_ARRAY(grids, PageDisplay::SetTypeGrid((TypeGrid::E)i));
}


static pchar FuncMapping(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[S_DISP_MAPPING]));

    SCPI_PROCESS_ARRAY(mapping, S_DISP_MAPPING = (DisplayMapping::E)i);
}


static pcharc smoothingsDirect[] =
{
    " 1",
    " 2",
    " 3",
    " 4",
    " 5",
    " 6",
    " 7",
    " 8",
    " 9",
    " 10",
    ""
};


static void SetSmoothing(int i)
{
    static const ENumSmoothing::E smoot[] =
    {
        ENumSmoothing::_10points,
        ENumSmoothing::_2points,
        ENumSmoothing::_3points,
        ENumSmoothing::_4points,
        ENumSmoothing::_5points,
        ENumSmoothing::_6points,
        ENumSmoothing::_7points,
        ENumSmoothing::_8points,
        ENumSmoothing::_9points,
        ENumSmoothing::Disable
    };

    S_DISP_ENUM_SMOOTH = smoot[i];
}


static pchar FuncSmoothing(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(smoothingsDirect[S_DISP_ENUM_SMOOTH]));

    static pcharc smoothings[] = { " 10", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", " 1", "" };

    SCPI_PROCESS_ARRAY(smoothings, SetSmoothing(i));
}


static void HintAccumulation(String *message)
{
    SCPI::ProcessHint(message, accumulationDirect);
}


static void HintAverages(String *message)
{
    SCPI::ProcessHint(message, averagesDirect);
}


static void HintBrightness(String *)
{

}


static void HintFPS(String *message)
{
    SCPI::ProcessHint(message, fps);
}


static void HintGrid(String *message)
{
    SCPI::ProcessHint(message, grids);
}


static void HintMapping(String *message)
{
    SCPI::ProcessHint(message, mapping);
}


static void HintSmoothing(String *message)
{
    SCPI::ProcessHint(message, smoothingsDirect);
}


static bool TestAccumulation()
{
    return false;
}


static bool TestAverages()
{
    return false;
}


static bool TestBrightness()
{
    return false;
}


static bool TestFPS()
{
    return false;
}


static bool TestGrid()
{
    return false;
}


static bool TestMapping()
{
    String commandLines(":DISPLAY:MAPPING LINEs%c", 0x0D);
    String commandDots(":DISPLAY:MApping dots%c", 0x0D);

    for (int i = 0; i < 10; i++)
    {
        SCPI_APPEND_STRING(commandLines); //-V814

        if (!S_DISP_MAPPING_IS_LINES)
        {
            SCPI_EXIT_ERROR();
        }

        SCPI_APPEND_STRING(commandDots); //-V814

        if (!S_DISP_MAPPING_IS_DOTS)
        {
            SCPI_EXIT_ERROR();
        }
    }

    return true;
}


static bool TestSmoothing()
{
    return false;
}
