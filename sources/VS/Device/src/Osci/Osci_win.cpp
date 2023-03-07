#include "defines.h"
#include "Hardware/Timer.h"
#include "Osci/Osci_win.h"
#include "Osci/ParametersOsci.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


namespace Osci
{
    // „итать данные канала в пам€ить data
    bool ReadDataChannel(Ch::E ch, uint8 *data);
}


bool OsciHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // ¬рем€ последнего чтени€ точки

    if(TIME_MS - timeLastRead >= TBase::TimePoint(S_TIME_BASE)) //-V2564
    {
        timeLastRead = TIME_MS;
        return true;
    }

    return false;
}


bool Osci::ReadDataChannel(Ch::E, uint8 *)
{
    return false;
}