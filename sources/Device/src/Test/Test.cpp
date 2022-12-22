// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>



static void RunTest(bool (*func)(), pchar message);


void Test::Run()
{
    if (!S_DBG_RUN_TEST)
    {
        return;
    }

    std::srand(TIME_MS);

    S_DBG_RUN_TEST = false;
    
    Display::Init();

    //RunTest(RAM::Test, "Тест ОЗУ");
    //
    //RunTest(ROM::Data::Test, "Тест ППЗУ");

    RunTest(SCPI::Test, "Тест SCPI");

    set.Save();

    Device::Reset();

    Display::DeInit();
}


static void RunTest(bool (*func)(), pchar message)
{
    Test::Display::AddMessage(message);

    if (!func())
    {
        Test::Display::AddMessage("!!! Ошибка !!!");
        while (true)
        {

        }
    }
}
