// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MessageMgr.h"
#include "Osci/Math/OsciMath.h"



void MessageMgr::OsciSettingsEffectOnAverageChanged()
{
    AveragerOsci::SettingChanged();
}
