// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"

// :FFT:DATA?
static pchar FFT_FuncData(pchar);
static void FFT_HintData(String *);
static bool FFT_TestData();

// :FFT:DISPLAY
static pchar FFT_FuncDisplay(pchar);
static void FFT_HintDisplay(String *);
static bool FFT_TestDisplay();

// :FFT:SCALE
static pchar FFT_FuncScale(pchar);
static void FFT_HintScale(String *);
static bool FFT_TestScale();

// :FFT:SOUCRE
static pchar FuncSource(pchar);
static void HintSource(String *);
static bool TestSource();

// :FFT:WINDOW
static pchar FuncWindow(pchar);
static void HintWindow(String *);
static bool TestWindow();


const StructSCPI SCPI::fft[] =
{
    SCPI_LEAF(":DATA?",   FFT_FuncData,    FFT_TestData,    "", FFT_HintData),
    SCPI_LEAF(":DISPLAY", FFT_FuncDisplay, FFT_TestDisplay, "", FFT_HintDisplay),
    SCPI_LEAF(":SCALE",   FFT_FuncScale,   FFT_TestScale,   "", FFT_HintScale),
    SCPI_LEAF(":SOURCE",  FuncSource,  TestSource,  "", HintSource),
    SCPI_LEAF(":WINDOW",  FuncWindow,  TestWindow,  "", HintWindow),
    SCPI_EMPTY()
};


static pchar FFT_FuncData(pchar buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::fft = true;

    SCPI_EPILOG(buffer);
}


static pString display[] =
{
    " ON",
    " OFF",
    ""
};

static void EnableFFT(int i)
{
    S_FFT_ENABLED = (i == 0);
}

static pchar FFT_FuncDisplay(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(display[S_FFT_ENABLED ? 0 : 1]));

    SCPI_PROCESS_ARRAY(display, EnableFFT(i));
}


static pString scale[] =
{
    " LOG",
    " LINEAR",
    ""
};

static pchar FFT_FuncScale(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(scale[S_FFT_SCALE]));

    SCPI_PROCESS_ARRAY(scale, S_FFT_SCALE = (ScaleFFT::E)i);
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
    SCPI_REQUEST(SCPI::SendAnswer(source[S_FFT_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_FFT_SOURCE = (SourceFFT::E)i);
}


static pString window[] =
{
    " RECTANGLE",
    " HAMMING",
    " BLACKMAN",
    " HANN",
    ""
};

static pchar FuncWindow(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(window[S_FFT_WINDOW]));

    SCPI_PROCESS_ARRAY(window, S_FFT_WINDOW = (WindowFFT::E)i);
}


static bool FFT_TestData()
{
    return false;
}


static bool FFT_TestDisplay()
{
    return false;
}


static bool FFT_TestScale()
{
    return false;
}


static bool TestSource()
{
    return false;
}


static bool TestWindow()
{
    return false;
}


static void FFT_HintData(String *)
{

}


static void FFT_HintDisplay(String *)
{

}


static void FFT_HintScale(String *)
{

}


static void HintSource(String *)
{

}


static void HintWindow(String *)
{

}
