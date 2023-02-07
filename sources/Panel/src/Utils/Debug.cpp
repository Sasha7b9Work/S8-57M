// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Debug.h"
#include "Hardware/Timer.h"



uint  Debug::timeStart = 0;
uint  Debug::timeCounter = 0;
uint  Debug::timeStartCounter = 0;
char *Debug::file = nullptr;
int   Debug::line = 0;
uint  Debug::size = 0;


void Debug::StartProfiling()
{
    timeStart = TIME_US;
}


void Debug::PointProfiling(char *name)
{
    LOG_WRITE("%s %d", name, TIME_US - timeStart);
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
