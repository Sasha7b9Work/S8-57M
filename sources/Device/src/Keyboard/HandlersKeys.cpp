#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/HandlersKeys.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Display/DisplayOsci.h"


// �������������� �������
static KeyEvent event;

// ����� ���������� ��������� ��������� ������ - �������� ��� ��������
static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);

// ������ ����������
static void Empty();

// ����� ���������� ��������� ��������. ���������� true, ���� ��������� � ���������� ��������� ������� ���������� �� ���������.
static bool CommonHandlerPage();

// ����� ���������� ��������� ��������� ����������
static void OnChangeParameterTime(pFuncVI, int);

// ��������� �������� ��� ��������� ���� � ����������� �� ����, ����� �������� ������ ��������
static void ShowHidePage(const Page *page);

static void ChangeRShift(Chan::E ch, int16 delta);

// ����������� ������� ������
static void OnTime();      // Key::Time
static void OnFunction();  // Key::Function
static void OnService();   // Key::Service
static void OnMeasures();  // Key::Measure
static void OnDisplay();   // Key::Display
static void OnMemory();    // Key::Memory
static void OnChannelA();  // Key::ChannelA
static void OnChannelB();  // Key::ChannelB
static void OnTrig();      // Key::Trig

static void OnArrow();     // Key::Left, Key::Up, Key::Right, Key::Down
static void OnEnter();     // Key::Enter
static void OnStart();     // Key::Start
static void OnTrigLev();   // Key::TrigLevLess, Key::TrigLevMore
static void OnRangeA();    // Key::RangeLessA, Key::RangeMoreA
static void OnRangeB();    // Key::RangeLessB, Key::RangeMoreB
static void OnRShiftA();   // Key::RShiftLessA, Key::RShiftMoreB
static void OnRShiftB();   // Key::RShiftLessB, Key::RShiftMoreB
static void OnTBase();     // Key::TBaseLess, Key::TBase::More
static void OnTShift();    // Key::TShiftLess, Key::TShiftMore
static void OnFX();        // Key::F1, Key::F2, Key::F3, Key::F4, Key::F5

// ����� ������� ��� ������� �� ������ �� ���������
static void CommonButtonPage(const Page *page);


void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Count][4] =
    { // Press       Repead       Release      Long
        {Empty,      Empty,       Empty,       Empty},       // None       
        {Empty,      Empty,       OnFunction,  Empty},       // Function   
        {Empty,      Empty,       OnMeasures,  Empty},       // Measure    
        {Empty,      Empty,       OnMemory,    Empty},       // Memory     
        {Empty,      Empty,       OnService,   Empty},       // Service    
        {Empty,      Empty,       OnChannelA,  OnChannelA},  // ChannelA   
        {Empty,      Empty,       OnChannelB,  OnChannelB},  // ChannelB   
        {Empty,      Empty,       OnTime,      OnTime},      // Time       
        {OnStart,    Empty,       Empty,       Empty},       // Start      
        {Empty,      Empty,       OnTrig,      OnTrig},      // Trig       
        {Empty,      Empty,       OnDisplay,   Empty},       // Display    
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeMoreA 
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeLessA 
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftMoreA
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftLessA
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeMoreB 
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeLessB 
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftMoreB
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftLessB
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseMore
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseLess
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftMore 
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftLess 
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevMore
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevLess
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Left
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Right
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Up
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Down
        {Empty,      Empty,       OnEnter,     OnEnter},     // Enter      
        {OnFX,       Empty,       OnFX,        OnFX},        // F1
        {OnFX,       Empty,       OnFX,        OnFX},        // F2
        {OnFX,       Empty,       OnFX,        OnFX},        // F3
        {OnFX,       Empty,       OnFX,        OnFX},        // F4
        {OnFX,       Empty,       OnFX,        OnFX}         // F5
    };

    Key::E       key = e.key;
    TypePress::E type = e.type;

    if (key < Key::Count && type < TypePress::None)
    {
        if (!CommonHandlerPage())
        {
            func[key][type]();
        }
    }
}

static void Empty()
{
}

static void ChangeRShift(Chan::E ch, int16 delta)
{
    if (!Device::InModeRecorder())
    {
        RShift::Change(ch, delta);

        DisplayOsci::SetFlagRedraw();
    }
}


static void OnRShiftA()
{
    OnChangeParameterChannel(ChangeRShift, ChanA, (event.key == Key::RShiftMoreA) ? 1 : -1);
}


static void OnRShiftB()
{
    OnChangeParameterChannel(ChangeRShift, ChanB, (event.key == Key::RShiftMoreB) ? 1 : -1);
}


static void OnRangeA()
{
    OnChangeParameterChannel(Range::Change, ChanA, (event.key == Key::RangeMoreA) ? 1 : -1);
}


static void OnRangeB()
{
    OnChangeParameterChannel(Range::Change, ChanB, (event.key == Key::RangeMoreB) ? 1 : -1);
}


static void OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    func(ch, delta);
}


static void OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}


static void OnTShift()
{
    int delta = (event.key == Key::TShiftMore) ? 1 : -1;

    if (Osci::IsRunning())
    {
        OnChangeParameterTime(TShift::Change, delta);
    }
    else
    {
        OnChangeParameterTime(DisplayOsci::ShiftInMemory::Change, delta);
    }
}


static void OnTBase()
{
    int delta = (event.key == Key::TBaseMore) ? 1 : -1;

    if (Device::InModeRecorder())
    {
        OnChangeParameterTime(Recorder::ScaleX::Change, delta);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, delta);
    }
}


static void OnFX()
{
    if (Menu::IsShown())
    {
        const Item *underKey = Menu::ItemUnderFunctionalKey(event.key);     // ������� ������� ����, ��������������� ������� ������

        if((underKey == &Item::empty && !Menu::OpenedItem()->IsPage()) && (event.IsRelease() || event.IsLong()) )
        {
            Menu::CloseOpenedItem();
        }
        else
        {
            underKey->HandlerFX(event.type);
        }
    }
}


static void OnArrow()
{
    Item *openedItem = Menu::OpenedItem();

    if (Menu::IsShown())
    {
        if (!openedItem->Is(TypeItem::Page))
        {
            openedItem->HandlerKey(event);
        }
    }
    else if (openedItem == PageFFT::Cursors::self) //-V2516
    {
        openedItem->HandlerKey(event);
    }
}


static bool CommonHandlerPage()
{
    bool result = false;

    if (Menu::IsShown())
    {
        Item *openedPage = Menu::OpenedItem();

        if (!openedPage->Is(TypeItem::Page))
        {
        }
        else if (Menu::CurrentItem()->HandlerKey(event))
        {
            result = true;
        }
        else
        {
            result = openedPage->HandlerKey(event);
        }
    }

    return result;
}


static void OnEnter()
{
    if (event.IsRelease())
    {
        if (!Menu::IsShown())
        {
            Menu::Show();
        }
        else
        {
            Menu::CloseOpenedItem();
        }
    }
    else if (event.IsLong()) //-V2516
    {
        return Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
}


static void OnTrigLev()
{
    TrigLevel::Change((event.key == Key::TrigLevMore) ? 1 : -1);
}


static void OnChannelA()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageChannelA::self);

        ShowHidePage(PageChannelA::self);
    }
    else if (event.IsLong()) //-V2516
    {
        RShift::Set(ChanA, 0);
    }
}


static void OnChannelB()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageChannelB::self);

        ShowHidePage(PageChannelB::self);
    }
    else if (event.IsLong()) //-V2516
    {
        RShift::Set(ChanB, 0);
    }
}


static void OnFunction()
{
    CommonButtonPage(PageFunction::self);

    ShowHidePage(PageFunction::self);
}


static void OnMeasures()
{
    CommonButtonPage(PageMeasures::self);

    ShowHidePage(PageMeasures::self);
}


static void OnMemory()
{
    if (FDrive::IsConnected() && S_MEM_MODE_BTN_MEMORY_IS_SAVE)
    {
        FDrive::SaveScreen();
    }
    else if(Device::InModeOsci()) //-V2516
    {
        CommonButtonPage(PageMemory::self);

        ShowHidePage(PageMemory::self);
    }
}


static void OnService()
{
    CommonButtonPage(PageService::self);

    ShowHidePage(PageService::self);
}


static void OnTime()
{
    CommonButtonPage(PageTime::self);

    if (event.IsRelease())
    {
        ShowHidePage(PageTime::self);
    }
    else if (event.IsLong()) //-V2516
    {
        if (Osci::IsRunning())
        {
            TShift::Reset();
        }
        else
        {
            DisplayOsci::ShiftInMemory::OnChangeTPos();
        }
    }
}


static void OnStart()
{
    if (Device::InModeTester())
    {
        Tester::StartStop();
    }
    else if(Device::InModeRecorder())
    {
        Recorder::OnPressStart();
    }
    else
    {
        Osci::OnPressButtonStart();
    }
}


static void OnTrig()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageTrig::self);

        ShowHidePage(PageTrig::self);
    }
    else if (event.IsLong()) //-V2516
    {
        TrigLevel::Set(S_TRIG_SOURCE, 0);
    }
}


static void OnDisplay()
{
    CommonButtonPage(PageDisplay::self);

    ShowHidePage(PageDisplay::self);
}


static void ShowHidePage(const Page *page)
{
    if (Menu::OpenedItem() == page)
    {
        Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
    else
    {
        Menu::SetMainPage(page);

        if (!Menu::IsShown())
        {
            Menu::Show();
        }
    }
}


static void CommonButtonPage(const Page *page)
{
    if(Menu::GetMainPage() != page)
    {
        if(!Menu::OpenedItem()->IsPage())
        {
            Menu::CloseOpenedItem();
        }
    }
}
