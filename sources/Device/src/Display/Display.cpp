// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Decoder_d.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/HAL/HAL.h"
#include "common/Display/Font/Font_d.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/Display/DisplayOsci.h"
#include "Recorder/DisplayRecorder.h"
#include "Utils/Values.h"
#include "Display/Primitives.h"
#include <cstring>


namespace Display
{
    static void EmptyFunc() { }


    bool Display::Message::waitKey = false;
    volatile bool Display::Message::running = false;
    bool Display::Breaker::powerOn = true;


    static pFuncVV funcOnHand = nullptr;
    static uint timeStart = 0;
    static pchar textWait = 0;
    static bool clearBackground = false;
    volatile static pFuncVV funcAdditionRender = EmptyFunc;   // �������������� ������� ���������. ����������� ����� ����������� ���������, �� ����� ������� EndScene;
    static bool inStateDraw = false;                        // true ��������, ��� ���������� ������� ���������
    static pFuncVV funcAfterUpdateOnce = EmptyFunc;

    // ��������� �������, ����������� ��� ���������� ����� ���������
    static void ExecuteFuncAfterUpdateOnce();

    // ��������� � ������ ����� �������
    static void LoadColorChannels();
}


void Display::Init()
{
    for(uint8 i = 0; i < Color::Count.value; i++)
    {
        Color(i).LoadValueRGB();
    }

    LoadBrightness();

    LoadColorChannels();
}


void Display::LoadColorChannels()
{
    for (int ch = 0; ch < 2; ch++)
    {
//        uint color = COLOR(Color::CHAN[ch].value);


    };
}


void Display::Render()
{
    static uint prevTime = 0;

    if(Device::InModeOsci() && (TIME_MS - prevTime < ENumSignalsInSec::TimeBetweenFramesMS()))
    {
        return;
    }

    prevTime = TIME_MS;

    DFont::SetSpacing();

    Color::ChangeFlash(false);

    DFont::Set(DTypeFont::_8);

    inStateDraw = true;

    static const pFuncVI funcs[Device::Mode::Count] =
    {
        DisplayOsci::Update,
        EmptyFuncVI,
        EmptyFuncVI,
        DisplayRecorder::Update
    };

    for (int field = 0; field < 5; field++)
    {
        funcs[(int)(Device::CurrentMode())](field);

        Console::Draw();

        funcAdditionRender();

        Painter::EndScene();
    }

    inStateDraw = false;

    ExecuteFuncAfterUpdateOnce();
}


bool Display::InProcess()
{
    return inStateDraw;
}


void Display::ExecuteFuncAfterUpdateOnce()
{
    funcAfterUpdateOnce();
    funcAfterUpdateOnce = EmptyFunc;
}


void Display::SetFuncAfterUpadteOnce(pFuncVV func)
{
    funcAfterUpdateOnce = func;
}


void Display::ShowWarning(pchar warn)
{
    Warnings::AddWarning(warn);
}


void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcOnHand = func;

    if (mode == DrawMode::Hand)
    {
        Timer::SetAndEnable(TypeTimer::Display, funcOnHand, 100);
    }
    else
    {
        Timer::Disable(TypeTimer::Display);
    }
}


void Display::Message::Hide()
{
    Display::SetDrawMode(DrawMode::Auto, nullptr);
    running = false;
    BufferButtons::Clear();
}


void Display::Message::Func()
{
    for (int field = 0; field < 5; field++)
    {
        if (clearBackground)
        {
            Painter::BeginScene(field, Color::BACK);
        }

        uint time = ((TIME_MS - timeStart) / 50) % 100;

        if (time > 50)
        {
            time = (100 - time);
        }

        int width = 200;
        int height = 80;
        int x = 160 - width / 2;
        int y = 120 - height / 2;

        Region(width, height).Fill(x, y, Color::BACK);
        Rectangle(width, height).Draw(x, y, Color::FILL);

        int length = DFont::GetLengthText(textWait);

        if (waitKey)
        {
            y += 10;
        }

        if (length < width)
        {
            Text(textWait).DrawInCenterRect(x, y, width, height - 20);
        }
        else
        {
            Text(textWait).DrawInRectWithTransfers(x + 11, y + 20, width - 22, height - 20);
        }

        char buf[100] = { 0 };

        if (!waitKey)
        {
            for (uint i = 0; i < time; i++)
            {
                std::strcat(buf, ".");
            }
        }

        Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

        Painter::EndScene();

        if (waitKey)
        {
            while (HAL_BUS::Panel::Receive()) {};

            DDecoder::Update();

            while (!BufferButtons::IsEmpty())
            {
                if (BufferButtons::Extract().IsRelease())
                {
                    Hide();
                }
            }
        }
    }
}



void Display::Message::Show(pchar text, bool eraseBackground)
{
    running = true;
    BufferButtons::Clear();
    timeStart = TIME_MS;
    textWait = text;
    clearBackground = eraseBackground;
    waitKey = false;
    Display::SetDrawMode(DrawMode::Hand, Func);
}


void Display::Message::ShowAndWaitKey(pchar text, bool eraseBackground)
{
    Show(text, eraseBackground);
    waitKey = true;
    while (running) {};
}


void Display::AddFunctionRender::Remove()
{
    funcAdditionRender = EmptyFunc;
}


void Display::AddFunctionRender::Set(pFuncVV func, uint time)
{
    if (func == 0)
    {
        func = EmptyFunc;
    }

    funcAdditionRender = func;

    if (time)
    {
        Timer::SetAndStartOnce(TypeTimer::RemoveAddFunction, AddFunctionRender::Remove, time);
    }
}


int Display::TimeMenuAutoHide()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[S_MENU_AUTOHIDE] * 1000;
}



bool Display::IsSeparate()
{
    return (S_MATH_MODE_DRAW_IS_SEPARATE || S_FFT_ENABLED);
}


uint ENumSignalsInSec::TimeBetweenFramesMS()
{
    static const uint time[] = { 1, 100, 200, 500, 1000 };

    return time[S_DISP_ENUM_FPS];
}


void Display::LoadBrightness()
{
    SBuffer(Command::Display_Brightness, (uint8)(S_DISP_BRIGHTNESS + 10)).Send();
}


void Display::Breaker::PowerOff()
{
    SBuffer(Command::Display_Brightness, 0).Send();

    powerOn = false;
}


bool Display::Breaker::PowerOn()
{
    if (!powerOn)
    {
        LoadBrightness();

        powerOn = true;

        return true;
    }

    return false;
}
