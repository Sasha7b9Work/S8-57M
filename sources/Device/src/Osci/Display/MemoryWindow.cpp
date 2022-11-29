#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"


static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, int length);

// ���������� ������� ������
static void DrawScreenArea();


void DisplayOsci::MemoryWindow::Draw() //-V2506
{
    if (CursorsMeasurements::NecessaryDraw())
    {
        return;
    }

    DrawScreenArea();

    Chan::E chans[2] = { S_DISP_LAST_AFFECTED_CHANNEL_IS_A ? ChanB : ChanA, S_DISP_LAST_AFFECTED_CHANNEL_IS_A ? ChanA : ChanB };

    for (int i = 0; i < 2; i++)
    {
        Chan::E ch = chans[i];
        if (S_CHANNEL_ENABLED(ch))
        {
            Color::CHAN[ch].SetAsCurrent();
            DrawDataInRect(X(), Y(), Width(), Height(), OUT(ch), ENumPointsFPGA::PointsInChannel());
        }
    }
}


static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, int length)
{
    if (PEAKDET_ENABLED(DS))
    {
        length *= 2;
    }

    int numIntervals = width + 1;                               // ���������� ����������, � ������� ����� �������� ��� ������ - ����������, ���������� ������������ �����
    float pointsInInterval = static_cast<float>(length) / numIntervals;      // ���������� �����, �������� � ����� ���������. //-V2564

    float stepY = static_cast<float>(height) / static_cast<float>(VALUE::MAX - VALUE::MIN);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = static_cast<int>(i * pointsInInterval + 0.5F);          // ��������� ����� � ��������� //-V2564
        int end = static_cast<int>((i + 1) * pointsInInterval + 0.5F) - 1;  // �������� ����� � ��������� //-V2564

        int min = 255;
        int max = 0;

        bool needDraw = true;

        for (int j = start; j <= end; j++)
        {
            if(data[j] == VALUE::NONE) //-V2563
            {
                needDraw = false;
                break;
            }
                
            if (data[j] < min) //-V2563
            {
                min = data[j]; //-V2563
            }
            if (data[j] > max) //-V2563
            {
                max = data[j]; //-V2563
            }
        }

        if(needDraw)
        {
            int deltaMIN = static_cast<int>(min * stepY + 0.5F); //-V2564
            int deltaMAX = static_cast<int>(max * stepY + 0.5F); //-V2564

            VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
        }
    }
}


static void DrawScreenArea()
{
    float relBegin = static_cast<float>(DisplayOsci::ShiftInMemory::Get()) / ENumPointsFPGA::PointsInChannel();          // ������������� ������ �������� ������� ������������ ���� ������ //-V2564
    float relWidth = static_cast<float>(Grid::Width() + 1) / ENumPointsFPGA::PointsInChannel();        // ������������� (������������ ���� ������� ����) ������ ������� ������� //-V2564

    int begin = static_cast<int>(relBegin * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // �������� ������ �������� ������� ������������ ������ ���� ������ //-V2564
    int width = static_cast<int>(relWidth * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // �������� ������ ������� ������� //-V2564

    Region(width, DisplayOsci::MemoryWindow::Height()).Fill(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::GRAY_20);

    Rectangle(width, DisplayOsci::MemoryWindow::Height()).Draw(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::FILL);
}


int DisplayOsci::MemoryWindow::Width()
{
    return 270;
}


int DisplayOsci::MemoryWindow::Height()
{
    return 17;
}


int DisplayOsci::MemoryWindow::X()
{
    return 0;
}


int DisplayOsci::MemoryWindow::Y()
{
    return ::Display::HEIGHT - 19;
}
