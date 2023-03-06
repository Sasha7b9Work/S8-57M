// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/common_defines.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "Osci/Osci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// *IDN?
static pchar FuncIDN(pchar);
static bool TestIDN();
static void HintIDN(String *);

// *RST
static pchar FuncReset(pchar);
static bool TestReset();
static void HintReset(String *);

// :HELP
static pchar FuncHelp(pchar);
static bool TestHelp();
static void HintHelp(String *);
static void ProcessHelp(const StructSCPI *strct, String message);

// :TEST
static pchar FuncTest(pchar);
static bool TestTest();
static void HintTest(String *);

// :CALIBRATOR:MODE
static pchar FuncCalibratorMode(pchar);
static bool TestCalibratorMode();
static void HintCalibratorMode(String *);

// :MEMORY:LENGTH
static pchar FuncMemoryLength(pchar);
static bool TestMemoryLength();
static void HintMemoryLength(String *);

// :MEMORY:SAVE
static pchar FuncMemorySave(pchar);
static bool TestMemorySave();
static void HintMemorySave(String *);

// :RUN
static pchar FuncRun(pchar);
static bool TestRun();
static void HintRun(String *);

// :STOP
static pchar FuncStop(pchar);
static bool TestStop();
static void HintStop(String *);

// :AUTOSEARCH
static pchar FuncAutoSearch(pchar);
static bool TestAutoSearch();
static void HintAutoSearch(String *);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",            FuncIDN,            TestIDN,            "ID request",                       HintIDN),
    SCPI_LEAF("*RST",             FuncReset,          TestReset,          "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",            FuncHelp,           TestHelp,           "Output of this help",              HintHelp),
    SCPI_LEAF(":TEST",            FuncTest,           TestTest,           "Run all tests",                    HintTest),
    SCPI_LEAF(":CALIBRATOR:MODE", FuncCalibratorMode, TestCalibratorMode, "",                                 HintCalibratorMode),
    SCPI_LEAF(":MEMORY:LENGTH",   FuncMemoryLength,   TestMemoryLength,   "",                                 HintMemoryLength),
    SCPI_LEAF(":MEMORY:SAVE",     FuncMemorySave,     TestMemorySave,     "",                                 HintMemorySave),
    SCPI_LEAF(":RUN",             FuncRun,            TestRun,            "",                                 HintRun),
    SCPI_LEAF(":STOP",            FuncStop,           TestStop,           "",                                 HintStop),
    SCPI_LEAF(":AUTOSEARCH",      FuncAutoSearch,     TestAutoSearch,     "",                                 HintAutoSearch),
    SCPI_NODE(":CHANNEL",         SCPI::channels),
    SCPI_NODE(":DISPLAY",         SCPI::display),
    SCPI_NODE(":FFT",             SCPI::fft),
    SCPI_NODE(":FREQMETER",       SCPI::freqmeter),
    SCPI_NODE(":KEY",             SCPI::key),
    SCPI_NODE(":MEASURE",         SCPI::measures),
    SCPI_NODE(":TIMEBASE",        SCPI::tBase),
    SCPI_NODE(":TRIG",            SCPI::trigger),
    SCPI_EMPTY()
};


static pchar FuncIDN(pchar buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("MNIPI, %s, %s, %s", IDENTIVICATOR, VERSION, CRC32_DEVICE).c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pchar FuncReset(pchar buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pchar FuncHelp(pchar buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    ProcessHelp(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pchar FuncTest(pchar buffer)
{
    static pchar const modes[2] =
    {
        " FULL",
        " FAST"
    };

    for (int i = 0; i < 2; i++)
    {
        pchar end = SCPI::BeginWith(buffer, modes[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            PageDebug::PageTests::OnPress_Run();

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static pchar FuncAutoSearch(pchar buffer)
{
    Osci::RunAutoSearch();
    return buffer;
}


static pcharc calibratorMode[] =
{
    " AC",
    " DC",
    ""
};

static void SetCalibratorMode(int i)
{
    S_SERV_CALIBRATOR_MODE = (Calibrator::Mode::E)i;
    
    FPGA::LoadCalibratorMode();
}

static pchar FuncCalibratorMode(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(calibratorMode[S_SERV_CALIBRATOR_MODE]));
    
    SCPI_PROCESS_ARRAY(calibratorMode, SetCalibratorMode(i));
}


static pcharc length[] =
{
    " 512",
    " 1K",
    " 2K",
    " 4K",
    " 8K",
    ""
};

static pchar FuncMemoryLength(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(length[set.mem.enumPoints]));

    SCPI_PROCESS_ARRAY(length, set.mem.enumPoints = (ENumPointsFPGA::E)i);
}


static pchar FuncMemorySave(pchar buffer)
{
    SCPI_PROLOG(buffer);

    if (FDrive::IsConnected())
    {
        FDrive::SaveScreen();
    }
    else
    {
        SCPI::SendAnswer("Error save screen. Connect flash-drive.");
    }

    SCPI_EPILOG(buffer);
}


static pchar FuncRun(pchar buffer)
{
    SCPI_PROLOG(buffer);

    Osci::Start(true);

    SCPI_EPILOG(buffer);
}


static pchar FuncStop(pchar buffer)
{
    SCPI_PROLOG(buffer);

    Osci::Stop();

    SCPI_EPILOG(buffer);
}


static void HintAutoSearch(String *)
{

}


static void HintCalibratorMode(String *)
{

}


static void HintMemoryLength(String *)
{

}


static void HintMemorySave(String *)
{

}


static void HintRun(String *)
{

}


static void HintStop(String *)
{

}


static void HintTest(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static bool TestAutoSearch()
{
    return false;
}


static bool TestCalibratorMode()
{
    return false;
}


static bool TestMemoryLength()
{
    return false;
}


static bool TestMemorySave()
{
    return false;
}


static bool TestRun()
{
    return false;
}


static bool TestStop()
{
    return false;
}


static bool TestIDN()
{
    return true;
}


static bool TestReset()
{
    return true;
}


static bool TestHelp()
{
    return true;
}


static bool TestTest()
{
    return true;
}


static void ProcessHelp(const StructSCPI *strct, String msg)
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            ProcessHelp(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg); //-V820
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }

        strct++;
    }
}
