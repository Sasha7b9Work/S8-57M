// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "FlashDrive/FlashDrive.h"
#include "FreqMeter/DisplayFreqMeter.h"
#include "Hardware/Battery.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Menu/Menu.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


namespace DisplayOsci
{
    // ������� ����, ��� ������� ��������� � ������ �����������
    static bool needRedraw = false;

    namespace DrawingValueParameter
    {
        static bool                     needDrawParameter = false;
        static DrawingValueParameter::E parameter;

        static void Disable();

        // ���������� ������ � �������������� �� ������ y
        static void DrawBoundedText(int y, const String *text, Color color);
    }
}


void DisplayOsci::Update()
{
    DFont::SetSpacing(1);

    if (needRedraw)
    {
        Painter::BeginScene(Color::BLACK);

        Grid::Draw();

        needRedraw = false;

        Accumulator::Reset();
    }

    PainterData::DrawData();

    Rectangle(Grid::Width(), Grid::ChannelHeight()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    HiPart::Draw();

    TPos::Draw();

    TShift::Draw();

    RShift::DrawBoth();

    TrigLevel::Draw();

    DrawingValueParameter::Draw();

    CursorsMeasurements::Draw();

    BottomPart::Draw(273, Grid::Bottom() + 1);

    DisplayFreqMeter::Update();

    TableMeasures::Draw();

    Menu::Draw();

    Warnings::Draw();
}


void DisplayOsci::SetFlagRedraw()
{
    needRedraw = true;
}


void DisplayOsci::BottomPart::Draw(int x0, int y0)
{
    DFont::Set(DTypeFont::_UGO2);

    // ������
    if (FDrive::IsConnected())
    {
    }

    if (VCP::connectedToUSB || VCP::cableUSBisConnected)
    {
        Char(SymbolUGO2::USB).Draw4SymbolsInRect(x0 + 72, y0 + 2, VCP::connectedToUSB ? Color::WHITE : Color::FLASH_01);
    }

    Color::FILL.SetAsCurrent();
    // ������� ��������
    if (PeakDetMode().IsEnabled())
    {
        Char('\x12').Draw(x0 + 38, y0 + 11);
        Char('\x13').Draw(x0 + 46, y0 + 11);
    }

    Battery::Draw(x0, y0);

    VLine(18).Draw(x0, y0 + 1, Color::FILL);
}


void DisplayOsci::DrawingValueParameter::Enable(DrawingValueParameter::E v) //-V2506
{
    if (S_FFT_ENABLED)                                                                     // �� ����� �������� ��� ���������� �������
    {
        return;
    }

    if (v == DrawingValueParameter::TrigLevel)
    {
        //        if (S_TRIG_MODE_FIND_IS_AUTO)
        //        {
        //            return;
        //        }
    }
    else if (!CursorsMeasurements::NecessaryDraw()) //-V2516
    {
        return;
    }

    needDrawParameter = true;

    parameter = v;

    Timer::SetAndStartOnce(TypeTimer::ShowLevelTrigLev, Disable, 2000);

    DisplayOsci::SetFlagRedraw();
}


void DisplayOsci::DrawingValueParameter::Draw()
{
    if (needDrawParameter)
    {
        int y = Grid::ChannelBottom() - 33;

        switch (parameter)
        {
        case TrigLevel:
        {
            float trigLevVal = RShift::ToAbs(S_TRIG_LEVEL_SOURCE, S_RANGE(S_TRIG_SOURCE)) * Divider::ToAbs(S_DIVIDER(S_TRIG_SOURCE)); //-V2564
            Voltage voltage(trigLevVal);
            String text("����� %s", voltage.ToString(true).c_str());
            DrawBoundedText(y, &text, Color::CHAN[S_TRIG_SOURCE]);
            break;
        }

        case RangeA:
        case RangeB:
        case RShiftA:
        case RShiftB:
        {
            Chan::E ch = ((parameter == RangeA) || (parameter == RShiftA)) ? ChanA : ChanB;
            char *channels[2] = { "1", "2" };
            String text("%s�: %s %s", channels[ch], Range::ToString(ch, S_DIVIDER(ch)), RShift::ToString(S_RSHIFT(ch), S_RANGE(ch), S_DIVIDER(ch)).c_str());
            DrawBoundedText(y, &text, Color::CHAN[ch]);
            break;
        }

        case TBase:
        case TShift:
            String text("�: %s %s", TBase::ToString(S_TIME_BASE), TShift::ToString(S_TIME_BASE).c_str());
            DrawBoundedText(y, &text, Color::FILL);
            break;
        }
    }
}


void DisplayOsci::DrawingValueParameter::DrawBoundedText(int y, const String *text, Color color)
{
    int length = DFont::GetLengthText(text->c_str()) + 6;
    int x = Grid::Left() + Grid::Width() / 2 - length / 2;
    Region(length, 16).DrawBounded(x, y, Color::BACK, Color::FILL);
    text->Draw(x + 4, y + 4, color);
}


void DisplayOsci::DrawingValueParameter::Disable()
{
    needDrawParameter = false;
}


int16 DisplayOsci::ShiftInMemory::Get()
{
    return set.disp._shiftInMemory;
}


void DisplayOsci::ShiftInMemory::Set(int16 shift)
{
    set.disp._shiftInMemory = shift;
}


void DisplayOsci::ShiftInMemory::OnChangeTPos()
{
    Set(Default(S_TPOS));
}


int16 DisplayOsci::ShiftInMemory::Default(TPos::E tPos) //-V2506
{
    if (tPos == TPos::Center)
    {
        return static_cast<int16>(ENumPointsFPGA::PointsInChannel() / 2 - Grid::Width() / 2);
    }
    else if (tPos == TPos::Right) //-V2516
    {
        return static_cast<int16>(ENumPointsFPGA::PointsInChannel() - Grid::Width() - 2);
    }

    return 0;
}


void DisplayOsci::ShiftInMemory::Change(int delta)
{
    int16 max = static_cast<int16>(ENumPointsFPGA::PointsInChannel() - Grid::Width());

    Set(Math::Add<int16>(Get(), static_cast<int16>(delta), 0, max));
}
