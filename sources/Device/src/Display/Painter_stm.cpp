#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"
#include "Display/Display.h"
#include <cstring>


namespace Painter
{
    static const int SIZE_BUFFER = Display::WIDTH * Display::HEIGHT / 2;

    uint8 buffer1[SIZE_BUFFER];
    uint8 buffer2[SIZE_BUFFER];

    uint8 *prev = nullptr;      // Указатель на буфер, отрисованный в прошлом кадре
    uint8 *draw = nullptr;      // В этом буфере рисуем в текущем буфере
                                // Используетя для расчёта дельты перед передачей в дисплей

    // Подготовить буфера к новому кадру
    void ToggleBuffers();
}


void Painter::Init()
{
}


void Painter::BeginScene(Color color)
{
    ToggleBuffers();

    color.SetAsCurrent();

    std::memset(draw, Color::GetCurent().value, SIZE_BUFFER);
}


void Painter::EndScene()
{
    if (prev == nullptr)    // Нет предыдущей отрисовки - передаём нарисованный буфер
    {

    }
    else                    // Если есть буфер, отрисованный в прошлом кадре, то пересылаем дельту
    {

    }
}


void Painter::DrawTesterData(uint8 , Color , const uint16 *, const uint8 *)
{
    // todo_paint
}


void Painter::ToggleBuffers()
{
    if (prev == nullptr && draw == nullptr)
    {
        draw = buffer1;
    }
    else
    {
        prev = draw;

        draw = (prev == buffer1) ? buffer2 : buffer1;
    }
}
