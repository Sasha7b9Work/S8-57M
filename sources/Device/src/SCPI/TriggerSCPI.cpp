// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


namespace SCPI_TRIG
{
    // :TRIG:INPUT
    static pchar FuncInput(pchar);
    static void HintInput(String *);
    static bool TestInput();

    // :TRIG:LEVEL
    static pchar FuncLevel(pchar);
    static void HintLevel(String *);
    static bool TestLevel();

    // :TRIG:MODE
    static pchar FuncMode(pchar);
    static void HintMode(String *);
    static bool TestMode();

    // :TRIG:POLARITY
    static pchar FuncPolarity(pchar);
    static void HintPolarity(String *);
    static bool TestPolarity();

    // :TRIG:SOURCE
    static pchar FuncSource(pchar);
    static void HintSource(String *);
    static bool TestSource();


    static pcharc inputs[] =
    {
        " FULL",
        " HF",
        " LF",
        ""
    };


    static pcharc trig_modes[] =
    {
        " AUTO",
        " NORMAL",
        " SINGLE",
        ""
    };


    static pcharc polarities[] =
    {
        " RISE",
        " FALL",
        ""
    };


    static pcharc sources[] =
    {
        " 1",
        " 2",
        ""
    };
}


const StructSCPI SCPI::trigger[] =
{
    SCPI_LEAF(":INPUT",    SCPI_TRIG::FuncInput,    SCPI_TRIG::TestInput,    "Filter selection by synchronization", SCPI_TRIG::HintInput),
    SCPI_LEAF(":LEVEL",    SCPI_TRIG::FuncLevel,    SCPI_TRIG::TestLevel,    "",                                    SCPI_TRIG::HintLevel),
    SCPI_LEAF(":MODE",     SCPI_TRIG::FuncMode,     SCPI_TRIG::TestMode,     "Set or query the trigger mode",       SCPI_TRIG::HintMode),
    SCPI_LEAF(":POLARITY", SCPI_TRIG::FuncPolarity, SCPI_TRIG::TestPolarity, "Sync polarity selection",             SCPI_TRIG::HintPolarity),
    SCPI_LEAF(":SOURCE",   SCPI_TRIG::FuncSource,   SCPI_TRIG::TestSource,   "Source selection",                    SCPI_TRIG::HintSource),
    SCPI_EMPTY()
};


pchar SCPI_TRIG::FuncInput(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(inputs[S_TRIG_INPUT]));

    SCPI_PROCESS_ARRAY(inputs, TrigInput::Set((TrigInput::E)i));
}


static void SendAnswerLevel()
{
    String answer("%d", S_TRIG_LEVEL_SOURCE / 2);
    SCPI::SendAnswer(answer.c_str());
}

pchar SCPI_TRIG::FuncLevel(pchar buffer)
{
    SCPI_REQUEST(SendAnswerLevel());

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        TrigLevel::Set(S_TRIG_SOURCE, (int16)(value * 2));

        return end_str + 1;
    }

    return nullptr;
}


pchar SCPI_TRIG::FuncMode(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(trig_modes[S_TRIG_START_MODE]));

    SCPI_PROCESS_ARRAY(trig_modes, TrigStartMode::Set((TrigStartMode::E)i));
}


pchar SCPI_TRIG::FuncPolarity(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(polarities[S_TRIG_POLARITY]));

    SCPI_PROCESS_ARRAY(polarities, TrigPolarity::Set((TrigPolarity::E)i));
}


pchar SCPI_TRIG::FuncSource(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(sources[S_TRIG_SOURCE]));

    SCPI_PROCESS_ARRAY(sources, TrigSource::Set((Ch::E)i));
}


void SCPI_TRIG::HintInput(String *message)
{
    SCPI::ProcessHint(message, inputs);
}


void SCPI_TRIG::HintLevel(String *)
{

}


void SCPI_TRIG::HintMode(String *message)
{
    SCPI::ProcessHint(message, trig_modes);
}


void SCPI_TRIG::HintPolarity(String *message)
{
    SCPI::ProcessHint(message, polarities);
}


void SCPI_TRIG::HintSource(String *message)
{
    SCPI::ProcessHint(message, sources);
}


bool SCPI_TRIG::TestInput()
{
    return false;
}


bool SCPI_TRIG::TestLevel()
{
    return false;
}


bool SCPI_TRIG::TestMode()
{
    return false;
}


bool SCPI_TRIG::TestPolarity()
{
    return false;
}


bool SCPI_TRIG::TestSource()
{
    return false;
}
