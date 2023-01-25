// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"


static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, int length);

// ���������� ������� ������
static void DrawScreenArea();


void DisplayOsci::MemoryWindow::Draw()
{
    if (Painter::CurrentField() != 4)
    {
        return;
    }

    if (Menu::IsShown())
    {
        return;
    }

    // \todo ��������� �������� ����� 30 ��

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
    float pointsInInterval = (float)length / numIntervals;      // ���������� �����, �������� � ����� ���������.

    float stepY = (float)height / (float)(VALUE::MAX - VALUE::MIN);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = (int)(i * pointsInInterval + 0.5F);          // ��������� ����� � ���������
        int end = (int)((i + 1) * pointsInInterval + 0.5F) - 1;  // �������� ����� � ���������

        int min = 255;
        int max = 0;

        bool needDraw = true;

        for (int j = start; j <= end; j++)
        {
            if(data[j] == VALUE::NONE)
            {
                needDraw = false;
                break;
            }
                
            if (data[j] < min)
            {
                min = data[j];
            }
            if (data[j] > max)
            {
                max = data[j];
            }
        }

        if(needDraw)
        {
            int deltaMIN = (int)(min * stepY + 0.5F);
            int deltaMAX = (int)(max * stepY + 0.5F);

            VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
        }
    }
}


static void DrawScreenArea()
{
    float relBegin = (float)DisplayOsci::ShiftInMemory::Get() / ENumPointsFPGA::PointsInChannel();          // ������������� ������ �������� ������� ������������ ���� ������
    float relWidth = (float)(Grid::Width() + 1) / ENumPointsFPGA::PointsInChannel();        // ������������� (������������ ���� ������� ����) ������ ������� �������

    int begin = (int)(relBegin * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // �������� ������ �������� ������� ������������ ������ ���� ������
    int width = (int)(relWidth * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // �������� ������ ������� �������

    Region(width, DisplayOsci::MemoryWindow::Height()).Fill(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::GRAY_20);

    Rectangle(width, DisplayOsci::MemoryWindow::Height()).Draw(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::FILL);
}


int DisplayOsci::MemoryWindow::Width()
{
    return 270 * 2;
}


int DisplayOsci::MemoryWindow::Height()
{
    return 17 * 2;
}


int DisplayOsci::MemoryWindow::X()
{
    return 0;
}


int DisplayOsci::MemoryWindow::Y()
{
    return ::Display::HEIGHT - 19 * 2;
}
