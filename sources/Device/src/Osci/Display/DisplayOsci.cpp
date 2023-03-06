// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font.h"
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
#include <cstdlib>


namespace DisplayOsci
{
    // Признак того, что дисплей нуждается в полной перерисовке
    static bool needRedraw = false;

    namespace DrawingValueParameter
    {
        static bool                     needDrawParameter = false;
        static DrawingValueParameter::E parameter;

        static void Disable();

        // Отобразить строку в прямоугольнике на высоте y
        static void DrawBoundedText(int y, const String *text, Color color);
    }
}


void DisplayOsci::Update(int field)
{
    Font::SetSpacing(1);

    if (needRedraw)
    {
        Painter::BeginScene(field, Color::BACK);

        Grid::Draw();

        if (field == 4)
        {
            needRedraw = false;
        }

        Accumulator::Reset();
    }

    PainterData::DrawData();

    Rectangle(Grid::Width(), Grid::ChannelHeight()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    HiPart::Draw(field);

    TPos::Draw();

    TShift::Draw();

    RShift::DrawBoth();

    TrigLevel::Draw();

    DrawingValueParameter::Draw();

    CursorsMeasurements::Draw();                        // Курсоры курсорных измерений

    BottomPart::Draw(540, Grid::Bottom() + 1, field);

    DisplayFreqMeter::Update();

    TableMeasures::Draw();

    Menu::Draw(field);

    Warnings::Draw();
}


void DisplayOsci::SetFlagRedraw()
{
    needRedraw = true;
}


void DisplayOsci::BottomPart::Draw(int x0, int y0, int field)
{
    if (field != 4)
    {
        return;
    }

    VLine(33).Draw(x0, y0 + 2, Color::FILL);

    Font::Set(TypeFont::UGO);

    Color::FILL.SetAsCurrent();

    if (VCP::client_connected || VCP::cable_connected)
    {
        SymbolUGO(SymbolUGO::USB).Draw(x0 + 65, y0 + 2, VCP::client_connected ? Color::WHITE : Color::FLASH_01);

        Battery::Draw(x0 + 5, y0 + 24, 50);
    }
    else
    {
        Battery::Draw(x0 + 5, y0 + 24, 85);
    }

    // Пиковый детектор
    if (PeakDetMode().IsEnabled())
    {
        // SymbolUGO(SymbolUGO::PEAK_DET_LEFT).Draw(x0 + 38, y0 + 11);
    }

    // Флешка
    if (FDrive::IsConnected())
    {

    }
}


void DisplayOsci::DrawingValueParameter::Enable(DrawingValueParameter::E v)
{
    if (S_FFT_ENABLED)                                                                     // Не будем рисовать при включённом спектре
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
    else if (!CursorsMeasurements::NecessaryDraw())
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
            float trigLevVal = RShift::ToAbs(S_TRIG_LEVEL_SOURCE, S_RANGE(S_TRIG_SOURCE)) * Divider::ToAbs(S_DIVIDER(S_TRIG_SOURCE));
            Voltage voltage(trigLevVal);
            String text("Синхр %s", voltage.ToString(true).c_str());
            DrawBoundedText(y, &text, Color::CHAN[S_TRIG_SOURCE]);
            break;
        }

        case RangeA:
        case RangeB:
        case RShiftA:
        case RShiftB:
        {
            Ch::E ch = ((parameter == RangeA) || (parameter == RShiftA)) ? ChA : ChB;
            char *channels[2] = { "1", "2" };
            String text("%sк: %s %s", channels[ch], Range::ToString(ch, S_DIVIDER(ch)), RShift::ToString(S_RSHIFT(ch), S_RANGE(ch), S_DIVIDER(ch)).c_str());
            DrawBoundedText(y, &text, Color::CHAN[ch]);
            break;
        }

        case TBase:
        case TShift:
            String text("р: %s %s", TBase::ToString(S_TIME_BASE), TShift::ToString(S_TIME_BASE).c_str());
            DrawBoundedText(y, &text, Color::FILL);
            break;
        }
    }
}


void DisplayOsci::DrawingValueParameter::DrawBoundedText(int y, const String *text, Color color)
{
    int length = Font::GetLengthText(text->c_str()) + 6;
    int x = Grid::Left() + Grid::Width() / 2 - length / 2;
    Region(length, 16).FillBounded(x, y, Color::BACK, Color::FILL);
    text->Draw(x + 4, y + 4, color);
}


void DisplayOsci::DrawingValueParameter::Disable()
{
    needDrawParameter = false;
}


int16 DisplayOsci::ShiftInMemory::Get()
{
    return set.disp.shiftInMemory;
}


void DisplayOsci::ShiftInMemory::Set(int16 shift)
{
    set.disp.shiftInMemory = shift;
}


void DisplayOsci::ShiftInMemory::OnChangeTPos()
{
    Set(Default(S_TPOS));
}


int16 DisplayOsci::ShiftInMemory::Default(TPos::E tPos)
{
    if (tPos == TPos::Center)
    {
        return (int16)(ENumPointsFPGA::PointsInChannel() / 2 - Grid::Width() / 2);
    }
    else if (tPos == TPos::Right)
    {
        return (int16)(ENumPointsFPGA::PointsInChannel() - Grid::Width() - 2);
    }

    return 0;
}


void DisplayOsci::ShiftInMemory::Change(int delta)
{
    int16 max = (int16)(ENumPointsFPGA::PointsInChannel() - Grid::Width());

    Set(Math::Add<int16>(Get(), (int16)delta, 0, max));
}
