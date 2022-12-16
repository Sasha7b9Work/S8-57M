// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Decoder_d.h"
#include "Display/Console.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Battery.h"
#include "Hardware/Beeper.h"
#include "Hardware/PowerBreaker.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/BufferButtons.h"
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"
#include "SCPI/SCPI.h"
#include "Settings/SettingsNRST.h"
#include "Display/Primitives.h"
#include <cstdlib>


// ��������� ���������� � ������������ � ������������� �������
static void SetCurrentMode();
// ���������� ����� ������ mode, ���� ������� �������� page ��� � ����� ������� �������� �������� ���� page
static bool SetCurrentMode(const Page *page, Device::Mode::E mode);

static Device::Mode::E currentMode = Device::Mode::Osci;



void Device::Init()
{
    HAL::Init();

    Console::Init();

    Timer::Init();

    PAUSE_ON_MS(500);

    HAL_TIM2::StartMultiMeasurement();

    Beeper::Init();

    Battery::Init();

    HAL_RTC::Init();

    VCP::Init();

    FPGA::Init();

    ROM::Init();

    Settings::Load();

    setNRST.Init();

    Display::Init();

    FreqMeter::Init();

    Osci::Init();

    Menu::Init();

    FDrive::Init();

    SetCurrentMode();

    //HAL_TIM5::Init();
}


void SetCurrentMode()
{
    if (!SetCurrentMode(PageRecorder::self, Device::Mode::Recorder))
    {
        Device::SetMode(Device::Mode::Osci);
    }
}


bool SetCurrentMode(const Page *page, Device::Mode::E mode)
{
    Item *opened = Menu::OpenedItem();

    if (opened &&
        (opened == const_cast<Page *>(page) || opened->ExistKeeper(page)))
    {
        Device::SetMode(mode);
        return true;
    }

    return false;
}


void Device::Update()
{
    Timer::StartMultiMeasurement();

    Osci::Update();

    {
        HAL_BUS::Panel::BeginScene();

        for (int i = 0; i < 5500; i++)
        {
            HAL_BUS::Panel::Send(Command::Null_command, 1);
        }

        int num_ms = (int)HAL_BUS::Panel::ElapsedUS() / 1000;

        LOG_WRITE("Test %d bytes, %d ms, %d Kb/sec",
            HAL_BUS::Panel::TransBytes(),
            num_ms,
            HAL_BUS::Panel::TransBytes() * 1000 / num_ms / 1024
        );
    }

    Text::BeginScene();
    HAL_BUS::Panel::BeginScene();

    uint time_start_ms = TIME_MS;

    Display::Update();

    LOG_WRITE("%d ms", TIME_MS - time_start_ms);

    int num_ms = (int)HAL_BUS::Panel::ElapsedUS() / 1000;
    int num_bytes = HAL_BUS::Panel::TransBytes();

    LOG_WRITE("%d bytes, %d ms, %d Kb/sec", num_bytes, num_ms, num_bytes * 1000 / (TIME_MS - time_start_ms) / 1024);

    FDrive::Update();

    while (HAL_BUS::Panel::Receive())
    {
    };

    SCPI::Update();

    DDecoder::Update();

    Menu::Update();

    PowerBreaker::Update();
}


Device::Mode::E Device::CurrentMode()
{
    return currentMode;
}


void Device::SetMode(Mode::E mode)
{
    if (mode != currentMode)
    {
        currentMode = mode;

        Osci::DeInit();
        Recorder::DeInit();

        switch (CurrentMode())
        {
        case Mode::Osci:
            Keyboard::Unlock();
            Osci::Init();
            break;

        case Mode::Tester:
        {
            static const Key::E keys[] =
            {
                Key::RangeMoreA, Key::RangeLessA, Key::RShiftMoreA, Key::RShiftLessA,
                Key::RangeMoreB, Key::RangeLessB, Key::RShiftMoreB, Key::RShiftLessB,
                Key::F1, Key::F2, Key::F3, Key::F4, Key::F5,
                Key::Left, Key::Right, Key::Up, Key::Down,
                Key::Start, Key::Enter, Key::Memory, Key::None
            };

            Keyboard::Lock(keys);
            break;
        }

        case Mode::Multimeter:
        {
            static const Key::E keys[] =
            {
                Key::F1, Key::F2, Key::F3, Key::F4, Key::F5,
                Key::Left, Key::Right, Key::Up, Key::Down,
                Key::Enter, Key::Memory, Key::None
            };

            Keyboard::Lock(keys);
            break;
        }

        case Mode::Recorder:
            Keyboard::Unlock();
            Recorder::Init();
            break;

        case Mode::Count:
            break;
        }
    }
}


bool Device::InModeOsci()
{
    return (CurrentMode() == Device::Mode::Osci);
}


bool Device::InModeRecorder()
{
    return (CurrentMode() == Device::Mode::Recorder);
}


void Device::Reset()
{
    HAL_NVIC::SystemReset();
}
