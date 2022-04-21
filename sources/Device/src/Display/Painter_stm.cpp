#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Containers/Buffer.h"
#include "Display/Display.h"
#include "common/Message.h"
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

    void SendDirectLines();
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
        SendDirectLines();
    }
    else                    // Если есть буфер, отрисованный в прошлом кадре, то пересылаем дельту
    {

    }
}


void Painter::SendDirectLines()
{
    for (int y = 0; y < Display::HEIGHT; y++)
    {
        uint8 num_segments = 0;
        uint8 color = 0;
        uint8 num_points = 0;

        uint8* line = draw + y * Display::WIDTH / 2;                  // Указатель на очередную передаваемую линию
        uint8* end = line + Display::WIDTH / 2;

        DynamicMessage<1024> message(Command::Paint_DirectLine);

        message.PushByte(0);                            // Сюда потом положим размер

        uint8 points = *line;

        {
            color = (uint8)(points & 0x0f);
            message.PushByte(color);
            num_points = 1;
            num_segments = 1;

            if ((points >> 4) == color)
            {
                num_points++;
            }
            else
            {
                message.PushByte(num_points);
                color = (uint8)(points >> 4);
                message.PushByte(color);
                num_points = 1;
                num_segments++;
            }
        }

        line++;

        while (line != end)
        {
            points = *line;

            if ((points & 0x0f) == color)
            {
                num_points++;
            }
            else
            {
                message.PushByte(num_points);
                color = (uint8)(points & 0x0f);
                num_points = 1;
                num_segments++;
            }

            if ((points >> 4) == color)
            {
                num_points++;
            }
            else
            {
                message.PushByte(num_points);
                color = (uint8)(points >> 4);
                message.PushByte(color);
                num_points = 1;
                num_segments++;
            }
        }

        message.PushByte(num_points);

        message.PushByte(1, num_segments);
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
