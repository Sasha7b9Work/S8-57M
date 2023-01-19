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
        // ���������� ������ ����� - ������������� � ����� ������
        static void DrawRightPart(int x0, int y0);

        // �������� ��������� ������������� ������ ��������� ������
        static void WriteTextVoltage(Chan::E ch, int x, int y);

        // ������� ������������ ��������� (text) � ��� �������� (number) �������� ��� ���������� (number_c)
        static void WriteStringAndNumber(pchar text, int x, int y, int number, pString number_c = nullptr);

        static void DrawTime(int x, int y);

        static void DrawFrequency(int x, int y);

        // ���������� �������������� �����
        static void DrawSeparators();

        // ���������� ������� ��������� � ��������� �������. ���������� �-���������� ������� �������� ���� ����������� �����������
        static int DrawMainParameters(int x, int y);

        static void WriteCursors();

        // ���������� ������ �������� ���������
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

    x = DrawMainParameters(x, Y0 + 1);

    Font::Set(TypeFont::Normal);

    DrawFrequency(x + 3, Y0 + 1);

    DrawTime(x + 3, Y0 + 19);

    Separator::Draw(540, Y0);

    DrawRightPart(540, Y0);

    WriteCursors();

    DrawPeakDet(x + 37, Y0 + 10);
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

    String("� : %s", TBase::ToString(S_TIME_BASE)).Draw(x, y0, Color::FILL);

    String(": %s", TShift::ToString().c_str()).Draw(x + 75, y0);

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        pString source[2] = { "1", "2" };

        String("c:").Draw(x, y1, Color::Trig());

        String(source[S_TRIG_SOURCE]).Draw(x + 10, y1, Color::Trig());
    }

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        static const SymbolUGO couple[] =
        {
            SymbolUGO(SymbolUGO::COUPLE_DC),
            SymbolUGO(SymbolUGO::COUPLE_DC),
            SymbolUGO(SymbolUGO::COUPLE_AC)
        };

        String(":").Draw(x + 25, y1);
        couple[S_TRIG_INPUT].Draw(x + 30, y1);

        static const SymbolUGO polar[] =
        {
            SymbolUGO(SymbolUGO::FRONT_RISE),
            SymbolUGO(SymbolUGO::FRONT_FALL)
        };

        String(":").Draw(x + 50, y1);
        polar[S_TRIG_POLARITY].Draw(x + 55, y1);

        static const SymbolUGO filtr[] =
        {
            SymbolUGO(SymbolUGO::FILTR_FULL),
            SymbolUGO(SymbolUGO::FILTR_LOW),
            SymbolUGO(SymbolUGO::FILTR_HI)
        };

        String(":").Draw(x + 75, y1);
        filtr[S_TRIG_INPUT].Draw(x + 80, y1);
    }

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        static const SymbolUGO mode[] =
        {
            SymbolUGO(SymbolUGO::START_MODE_AUTO),
            SymbolUGO(SymbolUGO::START_MODE_HAND),
            SymbolUGO(SymbolUGO::START_MODE_SINGLE)
        };

        String(":").Draw(x + 120, y1);
        mode[S_TRIG_START_MODE].Draw(x + 125, y1);
    }

    Font::Set(TypeFont::Small);

    x += 165;

    Separator::Draw(x - 3, y0 - 1);

    if (S_MEM_MODE_WORK_IS_DIR)
    {
        int d = 8;
        WriteStringAndNumber("������", x, y0 - 6 + d, 0, ENumAccum::ToString(S_DISP_ENUM_ACCUM));
        WriteStringAndNumber("������", x, y0 + 6 + d, S_OSCI_NUM_AVERAGE);
        WriteStringAndNumber("�����", x, y0 + 18 + d, S_DISP_NUM_SMOOTH);
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

    String((ch == ChanA) ? "1�" : "2�").Draw(x + 1, y, colorDraw);
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

    Text(buffer).DrawRelativelyRight(x + 75, y);
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
        SymbolUGO(SymbolUGO::F_T).Draw(x, y, Color::FILL);

        char mesFreq[20] = "=";

        float freq = FreqMeter::GetFrequency();

        if (freq == -1.0F)
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, Frequency(freq).ToString().c_str());
        }

        String(mesFreq).Draw(x + 15, y, Color::FILL);
    }
}


void DisplayOsci::HiPart::DrawRightPart(int x0, int y0)
{
    // �������������
    static const int xses[3] = { 280, 271, 251 };

    int x = xses[S_MEM_MODE_WORK];

    if (!S_MEM_MODE_WORK_IS_RAM)
    {
        x += 2;

        if (Trig::SyncPulse())
        {
            Region(35, 25).Fill(x0 + 5, y0 + 5, Color::FILL);
            String("��").Draw(x0 + 12, y0 + 8, Color::BACK);
        }
    }

    Separator::Draw(x0 + 43, y0);

    // ����� ������
    if (!S_MEM_MODE_WORK_IS_DIR)
    {
        static const pString strs[] =
        {
            "���",
            "����",
            "����"
        };

        x += 20;
        String("�����").Draw(x, -1);
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
                if (delta != 0.0F)
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
        SymbolUGO(SymbolUGO::PEAK_DET_LEFT).Draw(x, y, Color::FILL);
    }
}
