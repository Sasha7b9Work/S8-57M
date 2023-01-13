// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstring>
#include <cstdio>


namespace DisplayOsci
{
    namespace HiPart
    {
        // Нарисовать правую часть - синхронизация и режим работы
        static void DrawRightPart(int x0, int y0);

        // Написать параметры вертикального тракта заданного канала
        static void WriteTextVoltage(Chan::E ch, int x, int y);

        // Выводит наименование параметра (text) и его числовое (number) значение или символьное (number_c)
        static void WriteStringAndNumber(pchar text, int x, int y, int number, pString number_c = nullptr);

        static void DrawTime(int x, int y);

        static void DrawFrequency(int x, int y);

        // Нарисовать разделительные линии
        static void DrawSeparators();

        // Записывает главные параметры в указанную позицию. Возвращает х-координату правого верхнего угла выведенного изображения
        static int DrawMainParameters(int x, int y);

        static void WriteCursors();

        // Нарисовать значок пикового детектора
        static void DrawPeakDet(int x, int y);

        struct Separator
        {
            static void Draw(int x, int y)
            {
                VLine(35).Draw(x, y, Color::FILL);
            }
        };
    }
}


void DisplayOsci::HiPart::Draw(int field)
{
    if (field != 0)
    {
        return;
    }

#define Y0 0

    int x = -1;

    DrawSeparators();

    x = DrawMainParameters(x, Y0 + 1); //-V2007

    Font::Set(TypeFont::Normal);

    DrawFrequency(x + 3, Y0 + 1);

    DrawTime(x + 3, Y0 + 19); //-V2007

    Separator::Draw(540, Y0);

    DrawRightPart(540, Y0);

    WriteCursors();

    DrawPeakDet(x + 37, Y0 + 10); //-V2007
}


void DisplayOsci::HiPart::DrawSeparators()
{
    HLine line(Grid::Left() - TableMeasures::GetDeltaGridLeft() - 1);

    line.Draw(1, Grid::ChannelBottom(), Color::SEPARATOR);
    line.Draw(1, Grid::FullBottom());

    HLine line2(::Display::WIDTH - Grid::Right() - 4);

    line2.Draw(Grid::Right() + 2, Grid::Bottom(), Color::FILL);
    line2.Draw(Grid::Right() + 2, Grid::ChannelBottom());
}


int DisplayOsci::HiPart::DrawMainParameters(int _x, int _y)
{
    if (CursorsMeasurements::NecessaryDraw())
    {
        _y = Grid::Bottom() + 3;
    }

    int x = _x;
    const int y0 = _y;
    const int dY = 18;
    int y1 = _y + dY;

    WriteTextVoltage(ChanA, x + 2, y0);
    WriteTextVoltage(ChanB, x + 2, y1);

    x += 191;

    Separator::Draw(x - 4, _y - 1);

    String("р\xa5%s", TBase::ToString(S_TIME_BASE)).Draw(x, y0, Color::FILL);

    String("\xa5%s", TShift::ToString().c_str()).Draw(x + 75, y0);

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        pString source[3] = { "1", "2", "\x82" };

        String("с\xa5\x10%s", source[S_TRIG_SOURCE]).Draw(x, y1, Color::Trig());
    }

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        static const pString couple[] = { "\x92", "\x92", "\x91" };

        static const pString polar[] =  { "\xa7", "\xa6" };

        String("\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[S_TRIG_INPUT], polar[S_TRIG_POLARITY]).Draw(x + 30, y1);

        static const pString filtr[] = { "\xb5\xb6", "\xb3\xb4", "\xb1\xb2" };

        int x1 = x + 85;
        Char(filtr[S_TRIG_INPUT][0]).Draw(x1, y1);
        Char(filtr[S_TRIG_INPUT][1]).Draw(x1 + 16, y1);
    }

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        static char mode[] =
        {
            '\xb7',
            '\xa0',
            '\xb0'
        };

        String("\xa5\x10%c", mode[S_TRIG_START_MODE]).Draw(x + 125, y1);
    }

    Font::Set(TypeFont::Small);

    x += 165;

    Separator::Draw(x - 3, y0 - 1);

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        WriteStringAndNumber("накопл", x, y0 - 6, 0, ENumAccum::ToString(S_DISP_ENUM_ACCUM));
        WriteStringAndNumber("усредн", x, y0 + 6, S_OSCI_NUM_AVERAGE);
        WriteStringAndNumber("сглаж", x, y0 + 18, S_DISP_NUM_SMOOTH);
    }

    x += 80;

    Separator::Draw(x, y0 - 1);

    return x;
}


void DisplayOsci::HiPart::WriteTextVoltage(Chan::E ch, int x, int y)
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return;
    }

    Color color = Color::CHAN[ch];

    bool inverse = S_INVERSE(ch);

    Range::E range = S_RANGE(ch);

    Color colorDraw = inverse ? Color::WHITE : color;
    if (inverse)
    {
        Region(91, 8).Fill(x, y, color);
    }

    x += 50;
    y += 50;

    String((ch == ChanA) ? "1к" : "2к").Draw(x + 1, y, colorDraw);
    String(":").Draw(x + 18, y);
    ModeCouple::UGO(S_MODE_COUPLE(ch)).Draw(x + 23, y);
    String(":").Draw(x + 37, y);
    String(Range::ToString(ch, S_DIVIDER(ch))).Draw(x + 45, y);
    String(":").Draw(x + 80, y);
    RShift::ToString(S_RSHIFT(ch), range, S_DIVIDER(ch)).Draw(x + 85, y);
}


void DisplayOsci::HiPart::WriteStringAndNumber(pchar text, int x, int y, int number, pString number_c)
{
    String(text).Draw(x, y, Color::FILL);

    const int SIZE = 100;
    char buffer[SIZE];

    if (number_c != nullptr)
    {
        std::snprintf(buffer, SIZE, "%s", number_c);
    }
    else if (number == 0)
    {
        std::snprintf(buffer, SIZE, "-");
    }
    else
    {
        std::snprintf(buffer, SIZE, "%d", number);
    }

    Text(buffer).DrawRelativelyRight(x + 80, y);
}


void DisplayOsci::HiPart::DrawTime(int x, int y)
{
    int dField = 20;
    int dSeparator = 4;

    PackedTime time = HAL_RTC::GetPackedTime();

    Color::FILL.SetAsCurrent();

    if (S_MEM_MODE_WORK_IS_ROM || S_MEM_MODE_WORK_IS_RAM)
    {
        if (DS)
        {
            y -= 9;
            time.day = TIME_DAY_DS;
            time.hours = TIME_HOURS_DS;
            time.minutes = TIME_MINUTES_DS;
            time.seconds = TIME_SECONDS_DS;
            time.month = TIME_MONTH_DS;
            time.year = TIME_YEAR_DS;
            Integer((int)time.day).ToString(false, 2).Draw(x, y);
            String(':').Draw(x + dField, y);
            Integer((int)time.month).ToString(false, 2).Draw(x + dField + dSeparator, y);
            String(':').Draw(x + 2 * dField + dSeparator, y);
            Integer((int)time.year + 2000).ToString(false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);

            y += 9;
        }
        else
        {
            return;
        }
    }

    Integer((int)time.hours).ToString(false, 2).Draw(x, y);
    String(':').Draw(x + dField, y);
    Integer((int)time.minutes).ToString(false, 2).Draw(x + dField + dSeparator, y);
    String(':').Draw(x + 2 * dField + dSeparator, y);
    Integer((int)time.seconds).ToString(false, 2).Draw(x + 2 * dField + 2 * dSeparator, y);
}


void DisplayOsci::HiPart::DrawFrequency(int x, int y)
{
    if (S_MEM_MODE_WORK_IS_DIR)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FreqMeter::GetFrequency();
        if (freq == -1.0F) //-V550 //-V2550
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, Frequency(freq).ToString().c_str());
        }

        String(mesFreq).Draw(x, y, Color::FILL); //-V2007
    }
}


void DisplayOsci::HiPart::DrawRightPart(int x0, int y0)
{
    // Синхроимпульс
    static const int xses[3] = { 280, 271, 251 };

    int x = xses[S_MEM_MODE_WORK];

    if (!S_MEM_MODE_WORK_IS_RAM)
    {
        x += 2;

        if (Trig::SyncPulse())
        {
            Region(35, 25).Fill(x0 + 5, y0 + 5, Color::FILL);
            String("СИ").Draw(x0 + 12, y0 + 8, Color::BACK);
        }
    }

    Separator::Draw(x0 + 43, y0);

    // Режим работы
    if (!S_MEM_MODE_WORK_IS_DIR)
    {
        static const pString strs[] =
        {
            "ИЗМ",
            "ПОСЛ",
            "ВНТР"
        };

        x += 20;
        String("режим").Draw(x, -1);
        Text(strs[S_MEM_MODE_WORK]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (!S_MEM_MODE_WORK_IS_RAM)
    {
        x += 329;
        
        int y = 1;

        switch (OsciStateWork::Current())
        {
        case OsciStateWork::Stopped:    Region(20, 20).Fill(x + 3, y + 3);          break;
        case OsciStateWork::Triggered:  SymbolUGO(SymbolUGO::PLAY).Draw(x, y);      break;
        case OsciStateWork::Awaiting:   Region(4, 10).Fill(x + 3, y + 3);
                                        Region(4, 10).Fill(x + 10, y + 3);          break;
        }
    }
}


void DisplayOsci::HiPart::WriteCursors()
{
    if (CursorsMeasurements::NecessaryDraw())
    {
        int x = 0;
        int y1 = 0;
        int y2 = 0 + 9;

        VLine separator(Grid::Top() - 3);

        separator.Draw(x, 1, Color::FILL);
        separator.Draw(102, 1);
        separator.Draw(203, 1);

        x += 3;
        Chan::E source = S_CURS_SOURCE;
        Color colorText = Color::CHAN[source];
        if (!CursorsControl::IsDisabledU())
        {
            String("1:").Draw(x, y1, colorText);
            String("2:").Draw(x, y2);
            x += 7;
            CursorsMeasurements::Voltage(source, 0).Draw(x, y1);
            CursorsMeasurements::Voltage(source, 1).Draw(x, y2);
            x = 49;
            float pos0 = MathFPGA::VoltageCursor(CursorsMeasurements::PosU(source, 0), S_RANGE(source), S_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(CursorsMeasurements::PosU(source, 1), S_RANGE(source), S_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0) * Divider::ToAbs(S_DIVIDER(source));
            String(":dU=").Draw(x, y1);
            Voltage(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            CursorsMeasurements::PercentsU(source).Draw(x + 10, y2);
        }

        x = 101;
        x += 3;
        if (!CursorsControl::IsDisabledT())
        {
            colorText.SetAsCurrent();
            String("1:").Draw(x, y1);
            String("2:").Draw(x, y2);
            x += 7;
            CursorsMeasurements::Time(source, 0).Draw(x, y1);
            CursorsMeasurements::Time(source, 1).Draw(x, y2);
            x = 153;

            float pos0 = MathFPGA::TimeCursor(CursorsMeasurements::PosT(source, 0), S_TIME_BASE);
            float pos1 = MathFPGA::TimeCursor(CursorsMeasurements::PosT(source, 1), S_TIME_BASE);
            float delta = std::fabsf(pos1 - pos0);
            String(":dT=").Draw(x, y1);
            Time(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            CursorsMeasurements::PercentsT(source).Draw(x + 8, y2);

            if (S_CURS_SHOW_FREQ)
            {
                int width = 65;
                x = Grid::Right() - width;
                Rectangle(width, 12).Draw(x, Grid::Top(), Color::FILL);
                Region(width - 2, 10).Fill(x + 1, Grid::Top() + 1, Color::BACK);
                String("1/dT=").Draw(x + 1, Grid::Top() + 2, colorText);
                if (delta != 0.0F) //-V550 //-V2550
                {
                    Frequency(1.0F / delta).ToString().Draw(x + 25, Grid::Top() + 2);
                }
            }
        }
    }
}


void DisplayOsci::HiPart::DrawPeakDet(int x, int y)
{
    if (PeakDetMode().IsEnabled())
    {
        Char(SymbolUGO2::PEAK_DET_LEFT).Draw(x, y, Color::FILL);
        Char(SymbolUGO2::PEAK_DET_RIGHT).Draw(x + 8, y);
    }
}
