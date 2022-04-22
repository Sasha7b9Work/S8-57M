// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Painter.h"
#include "Display/Colors.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Painter_common.h"
#include "Display/MemPainter.h"


void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
//        LOG_ERROR_TRACE("������������ �������� deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // ���� ����� ����� �������� �� � ������ ������
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            MemPainter::DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        MemPainter::DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


Color Painter::GetColor()
{
    return currentColor;
}
