// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Settings.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/Osci.h"


void Settings::Load(bool _default)
{
    if(_default || !ROM::Settings::Load())
    {
        set = Settings::defaultSettings;
        Osci::Init();
        Menu::Init();
    }
}


void Settings::Save()
{
    ROM::Settings::Save();
}
