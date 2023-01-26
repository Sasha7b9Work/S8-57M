// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Hardware/HAL/HAL.h"
#include "common/Command.h"
#include <cstdlib>


bool DisplayOsci::PainterData::needSendToSCPI_FFT = false;


namespace DisplayOsci
{
    namespace PainterData
    {
        // Нарисовать актуальные данные - соответствующие текущим установкам
        static void DrawCurrent();

        // Нарисовать данные из ОЗУ
        static void DrawRAM();

        // Нарисовать данные из ППЗУ
        static void DrawROM();

        static void DrawChannel(Chan::E ch);

        static void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

        // mode : 0 - точки, 1 - линии
        static void DrawPeakDetOff(Chan::E ch, int center, const uint8 *data, float scale, int x, int mode);

        static void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);
        static void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

        static void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

        // Нарисовать спектр
        static void DrawSpectrum();

        static void DrawSpectrum(const uint8 *dataIn, int numPoints, Chan::E ch);

        static void DrawSpectrumChannel(const float *spectrum, Color color);

        static void WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1);
    }
}


void DisplayOsci::PainterData::DrawData()
{
    if (SCPI::Sender::fft)
    {
        SCPI::Sender::fft = false;
        needSendToSCPI_FFT = true;
    }


    if (AutoMeasurements::DataIsSetting())
    {
        static const pFuncVV func[ModeWork::Count] =
        {
            DrawCurrent,
            DrawRAM,
            DrawROM
        };
    
        func[S_MEM_MODE_WORK]();
    }

    DrawSpectrum();

    Accumulator::NextFrame();

    needSendToSCPI_FFT = false;
}


void DisplayOsci::PainterData::DrawCurrent()
{
    if (DS == nullptr)
    {
        return;
    }

    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        DrawChannel(ChanB);
        DrawChannel(ChanA);
    }
    else
    {
        DrawChannel(ChanA);
        DrawChannel(ChanB);
    }

    // \todo Время отрисовки - 30 мс
    DisplayOsci::MemoryWindow::Draw();
}


void DisplayOsci::PainterData::DrawRAM()
{
    DrawChannel(ChanA);
    DrawChannel(ChanB);
    DisplayOsci::MemoryWindow::Draw();
}


void DisplayOsci::PainterData::DrawSpectrumChannel(const float *spectrum, Color color)
{
    color.SetAsCurrent();
    int gridLeft = Grid::Left();
    int gridBottom = Grid::MathBottom();
    int gridHeight = Grid::MathHeight();

    for (int i = 0; i < 256; i++)
    {
        int height = (int)(gridHeight * spectrum[i]);

        VLine line(height);

        line.Draw(gridLeft + i * 2, gridBottom - height);
        line.Draw(gridLeft + i * 2 + 1, gridBottom - height);
    }
}


void DisplayOsci::PainterData::WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259 * 2;
    int y = Grid::ChannelBottom() + 5 * 2;
    int dY = 10 * 2;

    char buffer[20];
    Color::FILL.SetAsCurrent();

    Text(AutoMeasurements::Freq2String(freq0, false, buffer)).Draw(x, y);

    y += dY;

    Text(AutoMeasurements::Freq2String(freq1, false, buffer)).Draw(x, y);

    if(ch == ChanA)
    {
        y += dY + 4;
    }
    else
    {
        y += dY * 6 + 8;
    }

    Color::CHAN[ch].SetAsCurrent();

    Text(S_FFT_SCALE_IS_LOG ? SU::Db2String(density0, 4, buffer) : AutoMeasurements::Float2String(density0, false, buffer)).Draw(x, y);

    y += dY;

    Text(S_FFT_SCALE_IS_LOG ? SU::Db2String(density1, 4, buffer) : AutoMeasurements::Float2String(density1, false, buffer)).Draw(x, y);
}


void DisplayOsci::PainterData::DrawSpectrum(const uint8 *dataIn, int numPoints, Chan::E ch)
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return;
    }

#undef SIZE_BUFFER
#define SIZE_BUFFER (1024 * 4)

    if (numPoints > SIZE_BUFFER)
    {
        numPoints = SIZE_BUFFER;
    }

    float dataR[SIZE_BUFFER];

    float freq0 = 0.0F;
    float freq1 = 0.0F;
    float density0 = 0.0F;
    float density1 = 0.0F;
    int y0 = 0;
    int y1 = 0;

    float *spectrum = (float *)std::malloc(numPoints * sizeof(float));

    if (spectrum)
    {
        int s = 2;

        VALUE::PointsToVoltage(dataIn, numPoints, RANGE_DS(ch), (int16)RSHIFT_DS(ch), dataR);

        MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1, ch);

        DrawSpectrumChannel(spectrum, Color::CHAN[ch]);

        WriteParametersFFT(ch, freq0, density0, freq1, density1);

        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + S_FFT_POS_CUR_0 * 2, Grid::ChannelBottom(), Color::GRID);
        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + S_FFT_POS_CUR_1 * 2, Grid::ChannelBottom());

        Rectangle(s * 2, s * 2).Draw(S_FFT_POS_CUR_0 * 2 + Grid::Left() - s, y0 - s, Color::FILL);
        Rectangle(s * 2, s * 2).Draw(S_FFT_POS_CUR_1 * 2 + Grid::Left() - s, y1 - s);

        std::free(spectrum);
    }
}


void DisplayOsci::PainterData::DrawSpectrum()
{
    if (!S_FFT_ENABLED || !DS)
    {
        return;
    }

    VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Right(), Grid::ChannelBottom(), Color::BACK);
    
    
    if (S_MEM_MODE_WORK_IS_DIR)
    {
        int numPoints = DS->BytesInChannel();
    
        if (numPoints > 1024)       // \todo Пока 8к и более не хочет считать
        {
            numPoints = 1024;
        }
    
        if (S_FFT_SOURCE_IS_A)
        {
            DrawSpectrum(OUT_A, numPoints, ChanA);
        }
        else if (S_FFT_SOURCE_IS_B)
        {
            DrawSpectrum(OUT_B, numPoints, ChanB);
        }
        else
        {
            if (S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
            {
                DrawSpectrum(OUT_B, numPoints, ChanB);
                DrawSpectrum(OUT_A, numPoints, ChanA);
            }
            else
            {
                DrawSpectrum(OUT_A, numPoints, ChanA);
                DrawSpectrum(OUT_B, numPoints, ChanB);
            }
        }
    
    }
    
    VLine(Grid::MathHeight()).Draw(Grid::Left(), Grid::ChannelBottom(), Color::FILL);
    HLine(290).Draw(Grid::Left(), Grid::MathBottom());
}


void DisplayOsci::PainterData::DrawROM()
{
    if(S_MEM_TYPE_SIGNAL_ROM_IS_CURRENT || S_MEM_TYPE_SIGNAL_ROM_IS_BOTH)
    {
        DrawCurrent();
    }

    if (S_MEM_TYPE_SIGNAL_ROM_IS_RECORDED || S_MEM_TYPE_SIGNAL_ROM_IS_BOTH)
    {
        Reader::ReadDataFromROM();
        AutoMeasurements::SetData();
        DrawCurrent();
    }
}


void DisplayOsci::PainterData::DrawChannel(Chan::E ch)
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return;
    }

    uint8 *data = OUT(ch);

    data += DisplayOsci::ShiftInMemory::Get();

    if (PEAKDET_ENABLED(DS))
    {
        data += DisplayOsci::ShiftInMemory::Get();
    }

    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();

    int left = Grid::Left();

    float scale = (float)Grid::Height() / (VALUE::MAX - VALUE::MIN);

    if (S_FFT_ENABLED)
    {
        center -= (Grid::Bottom() - Grid::Top()) / 4;
        scale /= 2.0F;
    }

    Buffer bufferP2P;
    int posSeparator = 0;

    if(Roller::NeedDraw())
    {
        posSeparator = Roller::FillScreenBuffer(ch, bufferP2P, Grid::Width());
        data = bufferP2P.data;
    }

    if (S_DISP_MAPPING_IS_LINES)
    {
        DrawModeLines(ch, left, center, data, scale);
    }
    else
    {
        DrawModePoints(ch, left, center, data, scale);
    }

    if(posSeparator > 0)
    {
        VLine(Grid::Height()).Draw(Grid::Left() + posSeparator, Grid::Top(), Color::GRID);
    }
}


void DisplayOsci::PainterData::DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    int x = left;

    if (PEAKDET_ENABLED(DS))
    {
        DrawModeLinesPeakDetOn(center, data, scale, x);
    }
    else
    {
        DrawPeakDetOff(ch, center, data, scale, x, 1);
    }
}


void DisplayOsci::PainterData::DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    if (PEAKDET_ENABLED(DS))
    {
        DrawModePointsPeakDetOn(center, data, scale, left);
    }
    else
    {
        DrawPeakDetOff(ch, center, data, scale, left, 0);
    }
}


void DisplayOsci::PainterData::DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        int min = (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int max = (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F);

        VLine(min - max).Draw(x++, max);
    }
}


void DisplayOsci::PainterData::DrawPeakDetOff(Chan::E ch, int center, const uint8 *data, float scale, int x, int mode)
{
    const int NUM_POINTS = Grid::Width();

    SBuffer buffer(Command::Paint_DrawSignal, (uint8)((mode << 1) | ((int)ch << 2)));   // 0,1

    buffer.Push(Point2(x, Grid::Top()));                    // 2-4
    buffer.Push(Point2(x, Grid::ChannelBottom()));          // 5-7
    buffer.Push((uint8 *)&NUM_POINTS, 2);                   // 8-10

    buffer.Send();

    for (int i = 0; i < NUM_POINTS; i++)
    {
        HAL_BUS::Panel::SendByte(*data++);
    }
}


void DisplayOsci::PainterData::DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        Pixel().Draw(x, (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F));
        Pixel().Draw(x, (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F));
        x++;
    }
}


BitSet64 DisplayOsci::PainterData::PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = DisplayOsci::ShiftInMemory::Get();
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}


BitSet64 DisplayOsci::PainterData::BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = DisplayOsci::ShiftInMemory::Get();
    retValue.sword1 = retValue.sword0 + 281;

    if (PEAKDET_ENABLED(DS))
    {
        retValue.sword1 += 281;
    }

    return retValue;
}
