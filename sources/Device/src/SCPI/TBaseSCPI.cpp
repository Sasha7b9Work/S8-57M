// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


namespace SCPI_TBASE
{
    // :TIMEBASE:MODE
    static pCHAR FuncMode(pCHAR);
    static bool TestMode();
    static void HintMode(String *);

    // :TIMEBASE:OFFSETBASE
    static pCHAR FuncOffsetBase(pCHAR);
    static bool TestOffsetBase();
    static void HintOffsetBase(String *);

    // :TIMEBASE:PEAKDET
    static pCHAR FuncPeakDet(pCHAR);
    static bool TestPeakDet();
    static void HintPeakDet(String *);

    // :TIMEBASE:SCALE:
    static pCHAR FuncScale(pCHAR);
    static bool TestScale();
    static void HintScale(String *);

    // :TIMEBASE:TPOS:
    static pCHAR FuncTPos(pCHAR);
    static bool TestTPos();
    static void HintTPos(String *);



    static pString tBaseNames[] =
    {
        " 2NS",
        " 5NS",
        " 10NS",
        " 20NS",
        " 50NS",
        " 100NS",
        " 200NS",
        " 500NS",
        " 1US",
        " 2US",
        " 5US",
        " 10US",
        " 20US",
        " 50US",
        " 100US",
        " 200US",
        " 500US",
        " 1MS",
        " 2MS",
        " 5MS",
        " 10MS",
        " 20MS",
        " 50MS",
        " 100MS",
        " 200MS",
        " 500MS",
        " 1S",
        " 2S",
        " 5S",
        " 10S",
        ""
    };


    static pString peakdets[] =
    {
        " OFF",
        " ON",
        ""
    };


    static pString tposes[] =
    {
        " LEFT",
        " CENTER",
        " RIGHT",
        ""
    };
}


const StructSCPI SCPI::tBase[] =
{
    SCPI_LEAF(":MODE",       SCPI_TBASE::FuncMode,       SCPI_TBASE::TestMode,       "",                        SCPI_TBASE::HintMode),
    SCPI_LEAF(":PEAKDET",    SCPI_TBASE::FuncPeakDet,    SCPI_TBASE::TestPeakDet,    "Peak detector control",   SCPI_TBASE::HintPeakDet),
    SCPI_LEAF(":OFFSETBASE", SCPI_TBASE::FuncOffsetBase, SCPI_TBASE::TestOffsetBase, "",                        SCPI_TBASE::HintOffsetBase),
    SCPI_LEAF(":SCALE",      SCPI_TBASE::FuncScale,      SCPI_TBASE::TestScale,      "Horizontal zoom control", SCPI_TBASE::HintScale),
    SCPI_LEAF(":TPOS",       SCPI_TBASE::FuncTPos,       SCPI_TBASE::TestTPos,       "Snap sync to screen",     SCPI_TBASE::HintTPos),
    SCPI_EMPTY()
};


static pString modes[] =
{
    " EQUAL",
    " REAL",
    ""
};

static void SetSampleType(int i)
{
    PageTime::SetSampleTime(static_cast<SampleType::E>(i));
}

pCHAR SCPI_TBASE::FuncMode(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_RAND_SAMPLE_TYPE]));

    SCPI_PROCESS_ARRAY(modes, SetSampleType(i));
}


static void AnswerOffsetBase()
{
    String answer("%d", set.time._shift);
    SCPI::SendAnswer(answer.c_str());
}

pCHAR SCPI_TBASE::FuncOffsetBase(pCHAR buffer)
{
    SCPI_REQUEST(AnswerOffsetBase());

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        TShift::Set(value);

        return end_str + 1;
    }

    return nullptr;
}


pCHAR SCPI_TBASE::FuncPeakDet(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(peakdets[S_PEAK_DET]));

    SCPI_PROCESS_ARRAY(peakdets, PeakDetMode::Set(static_cast<PeakDetMode::E>(i)));
}


pCHAR SCPI_TBASE::FuncScale(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[S_TIME_BASE]));

    SCPI_PROCESS_ARRAY(tBaseNames, TBase::Set(static_cast<TBase::E>(i)));
}


pCHAR SCPI_TBASE::FuncTPos(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tposes[S_TPOS]));

    SCPI_PROCESS_ARRAY(tposes, TPos::Set(static_cast<TPos::E>(i)));
}


bool SCPI_TBASE::TestMode()
{
    return true;
}


bool SCPI_TBASE::TestOffsetBase()
{
    return false;
}


bool SCPI_TBASE::TestPeakDet()
{
    return true;
}


bool SCPI_TBASE::TestScale()
{
   return true;
}


bool SCPI_TBASE::TestTPos()
{
    return true;
}


void SCPI_TBASE::HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


void SCPI_TBASE::HintOffsetBase(String *)
{

}


void SCPI_TBASE::HintPeakDet(String *message)
{
    SCPI::ProcessHint(message, peakdets);
}


void SCPI_TBASE::HintScale(String *message)
{
    SCPI::ProcessHint(message, tBaseNames);
}


void SCPI_TBASE::HintTPos(String *message)
{
    SCPI::ProcessHint(message, tposes);
}
