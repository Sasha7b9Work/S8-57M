// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


Queue<WarningStruct> warnings;


// Удалить предупреждения с истёкшим времени жизни
static void RemoveOld();
// Возвращает true, 
static bool BackMessagesSame();

static void DrawMessages();



void Warnings::AddWarning(pchar warning)
{
    warnings.Push(WarningStruct(warning));

    if(BackMessagesSame())
    {
        warnings[warnings.Size() - 2].timeStart = warnings[warnings.Size() - 1].timeStart;
        warnings.Back();
    }
}


static bool BackMessagesSame()
{
    if (warnings.Size() < 2)
    {
        return false;
    }

    return (std::strcmp(warnings[warnings.Size() - 2].message.c_str(), warnings[warnings.Size() - 1].message.c_str()) == 0);
}


void Warnings::Draw()
{
    if(warnings.Size())
    {
        RemoveOld();

        if(warnings.Size())
        {
            DrawMessages();
        }
    }
}


static void RemoveOld()
{
    while (!warnings.IsEmpty() && warnings[0].IsDead())
    {
        warnings.Front();
    }
}


static void DrawMessages()
{
    Font::Set(TypeFont::Normal);

    int y = Grid::BottomForWarnings();                   // Координата y нижнего левого угла прямоугольника, в котором будет отрисовано очередное сообщение

    for (int i = warnings.Size() - 1; i >= 0; i--)
    {
        y -= warnings[i].Height(Grid::Width());

        if (y < Grid::Top())
        {
            break;
        }

        warnings[i].Draw(Grid::Left(), y);
    }
}


bool Warnings::IsShown()
{
    return (warnings.Size() != 0);
}


WarningStruct::WarningStruct(pchar msg)
{
    message.Set(TypeConversionString::None, msg);
    timeStart = TIME_MS;
}


WarningStruct::WarningStruct(const WarningStruct &s)
{
    message.Set(TypeConversionString::None, s.message.c_str());
    timeStart = s.timeStart;
}


WarningStruct &WarningStruct::operator=(const WarningStruct &s)
{
    timeStart = s.timeStart;
    message.Set(TypeConversionString::None, s.message.c_str());
    return *this;
}


WarningStruct::~WarningStruct()
{
    message.Free();
}


bool WarningStruct::IsDead() const
{
    return (TIME_MS - timeStart) > 3500;
}

int WarningStruct::Height(int) const
{
    return 10;
}

void WarningStruct::Draw(int x, int y) const
{
    int width = Font::GetLengthText(message.c_str());

    Region(width + 3, 10).FillBounded(x, y, Color::FLASH_10, Color::FILL);

    Text(message).Draw(x + 2, y + 1, Color::FLASH_01);
}
