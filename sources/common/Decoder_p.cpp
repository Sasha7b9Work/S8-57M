// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "structs.h"
#include "Decoder_p.h"
#include "Display/Display.h"
#include "Display/Text/Text.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "Display/BackBuffer.h"
#include "Settings/Settings.h"
#include "Display/Font/Font_p.h"
#include "Log.h"


namespace PDecoder
{
    typedef bool(*pFuncBU8)(uint8);

    // Выполняемая функция
    static pFuncBU8 curFunc;

    // Текущий байт выполняемой функции
    static int step = 0;

    // Обработка запроса на изображение экрана
    static bool FuncScreen(uint8);

    static bool E(uint8) { return true; }

    static bool InButtonPress(uint8);

    static bool BeginScene(uint8);

    static bool EndScene(uint8);

    static bool SetColor(uint8);

    static bool FillRegion(uint8);

    static bool DrawText(uint8);

    static bool SetPalette(uint8);

    static bool DrawRectangle(uint8);

    static bool DrawVLine(uint8);

    static bool SetFont(uint8);

    static bool SetPoint(uint8);

    static bool DrawLine(uint8);

    static bool DrawHLine(uint8);

    static bool DisplayBrightness(uint8);

    static bool DrawVPointLine(uint8);

    static bool DrawHPointLine(uint8);

    // Установка моноширинного вывода шрифта
    static bool SetMinWidthFont(uint8);

    // Устанавливает расстояние между символами при выводе текста
    static bool SetTextSpacing(uint8);

    static bool DrawVCursor(uint8);
    static bool DrawHCursor(uint8);

    static bool SetColorChannel(uint8);

    // Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();

    static bool DrawSignal(uint8);

    namespace Converter
    {
        static float scale = 0.0f;
        static int y_top = 0;

        static void Prepare(int bottom, int top)
        {
            y_top = top;

            scale = (float)(bottom - top) / 250.0f;
        }

        static int CacluateY(int data)
        {
            return (int)(y_top + (250 - (data - 2)) * scale);
        }
    }
}


void PDecoder::AddData(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    command[Command::Count] =
    {
        E,
        InButtonPress,
        BeginScene,
        EndScene,
        SetColor,
        FillRegion,
        DrawText,
        SetPalette,
        DrawRectangle,
        DrawVLine,
        DrawHLine,
        SetFont,
        SetPoint,
        DrawLine,
        DisplayBrightness,
        FuncScreen,
        DrawVPointLine,
        DrawHPointLine,
        SetMinWidthFont,
        SetTextSpacing,
        E,
        DrawSignal,
        DrawVCursor,
        DrawHCursor,
        SetColorChannel
    };

    if (step == 0)
    {
        if (data < Command::Count)
        {
            curFunc = command[data].func;
        }
        else
        {
            FinishCommand();
            return;     // Простенькая защита от ошибок
        }
    }

    if (curFunc(data))
    {
        FinishCommand();
    }
    else
    {
        step++;
    }
}


bool PDecoder::InButtonPress(uint8)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        return false;
    }
    if (step == 2)
    {
    }
    return true;
}


bool PDecoder::BeginScene(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    BackBuffer::BeginPaint((int)data);

    return true;
}


bool PDecoder::DisplayBrightness(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    HAL_DAC2::SetValue(data);

    return true;
}


bool PDecoder::EndScene(uint8)
{
    BackBuffer::EndPaint();
    return true;
}


bool PDecoder::SetColor(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    Color(data).SetAsCurrent();

    return true;
}


bool PDecoder::FuncScreen(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    BackBuffer::SendRow(data);

    return true;
}


bool PDecoder::FillRegion(uint8 data)
{
    static Point2 pos;
    static Point2 size;

    bool result = false;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos.Append(data);
        break;
    case 4:
    case 5:
        size.Append(data);
        break;
    case 6:
        size.Append(data);
        BackBuffer::FillRegion(pos.X(), pos.Y(), size.Width(), size.Height());
        pos.Reset();
        size.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::DrawRectangle(uint8 data)
{
    static Point2 pos;
    static Point2 size;

    bool result = false;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos.Append(data);
        break;
    case 4:
    case 5:
        size.Append(data);
        break;
    case 6:
        size.Append(data);
        BackBuffer::DrawRectangle(pos.X(), pos.Y(), size.Width(), size.Height());
        pos.Reset();
        size.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::DrawVLine(uint8 data)
{
    static Point2 pos1;
    static Point2 pos2;

    if (step > 0)
    {
        if (step < 4)
        {
            pos1.Append(data);
        }
        else
        {
            pos2.Append(data);

            if (step > 5)
            {
                BackBuffer::DrawVLine(pos1.X(), pos1.Y(), pos2.Y());
                pos1.Reset();
                pos2.Reset();

                return true;
            }
        }
    }

    return false;
}


bool PDecoder::DrawHLine(uint8 data)
{
    static Point2 pos1;
    static Point2 pos2;

    if (step > 0)
    {
        if (step < 4)
        {
            pos1.Append(data);
        }
        else
        {
            pos2.Append(data);

            if (step > 5)
            {
                BackBuffer::DrawHLine(pos1.Y(), pos1.X(), pos2.X());
                pos1.Reset();
                pos2.Reset();

                return true;
            }
        }
    }

    return false;
}


bool PDecoder::DrawVPointLine(uint8 data)
{
    static Point2 pos;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos.Append(data);
        break;
    case 4:
        delta = data;
        break;
    case 5:
        count = data;
        BackBuffer::DrawVPointLine(pos.X(), pos.Y(), delta, count);
        pos.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::DrawHPointLine(uint8 data)
{
    static Point2 pos;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos.Append(data);
        break;
    case 4:
        delta = data;
        break;
    case 5:
        count = data;
        BackBuffer::DrawHPointLine(pos.X(), pos.Y(), delta, count);
        pos.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::DrawVCursor(uint8 data)
{
    static Point2 coord;
    static Point2 height_skip;

    if (step == 0)
    {
    }
    else if (step < 4)
    {
        coord.Append(data);
    }
    else
    {
        height_skip.Append(data);

        if (step == 6)
        {
            BackBuffer::DrawVCursor(coord.X(), coord.Y(),
                height_skip.X(), height_skip.Y());

            coord.Reset();
            height_skip.Reset();

            return true;
        };
    }

    return false;
}


bool PDecoder::DrawHCursor(uint8 data)
{
    static Point2 coord;
    static Point2 width_skip;

    if (step == 0)
    {
    }
    else if (step < 4)
    {
        coord.Append(data);
    }
    else
    {
        width_skip.Append(data);

        if (step == 6)
        {
            BackBuffer::DrawHCursor(coord.X(), coord.Y(), width_skip.X(), width_skip.Y());

            coord.Reset();
            width_skip.Reset();

            return true;
        }
    }

    return false;
}


bool PDecoder::SetMinWidthFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    Text::SetMinWidthFont(data);

    return true;
}


bool PDecoder::SetTextSpacing(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    Text::SetSpacing(data);

    return true;
}


bool PDecoder::DrawLine(uint8 data)
{
    static Point2 pos0;
    static Point2 pos1;

    bool result = false;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos0.Append(data); //-V525
        break;
    case 4:
    case 5:
        pos1.Append(data);
        break;
    case 6:
        pos1.Append(data);
        BackBuffer::DrawLine(pos0.X(), pos0.Y(), pos1.X(), pos1.Y());
        pos0.Reset();
        pos1.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::SetPoint(uint8 data)
{
    static Point2 pos;

    if (step > 0)
    {
        pos.Append(data);

        if (step > 2)
        {
            BackBuffer::FillRegion(pos.X(), pos.Y(), 1, 1);

            pos.Reset();

            return true;
        }
    }

    return false;
}


bool PDecoder::DrawText(uint8 data)
{
    static Point2 pos;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        pos.Append(data);
        break;
    case 4:
        numSymbols = data;
        readingSymbols = 0;
        buffer = new char[(uint)(numSymbols + 1)];
        break;
    default:
        buffer[readingSymbols++] = (char)(data);
        if (readingSymbols == numSymbols)
        {
            buffer[readingSymbols] = 0;
            Text::Draw(pos.X(), pos.Y(), buffer, 2);
            pos.Reset();
            delete[]buffer;
            return true;
        }
        break;
    }
    return false;
}


bool PDecoder::SetPalette(uint8 data)
{
    static uint8 numColor;
    static uint valueColor;

    bool result = false;

    switch (step)
    {
    case 0:                                                 break;
    case 1: numColor = data;                                break;
    case 2: valueColor = data;                              break;
    case 3: valueColor |= (uint)(data) << 8;     break;
    case 4: valueColor |= (uint)(data) << 16;    break;
    case 5: valueColor |= (uint)(data) << 24;
        Color(numColor).SetValue(valueColor);
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::SetFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    PFont::Set((PTypeFont::E)data);

    return true;
}


bool PDecoder::SetColorChannel(uint8 data)
{
    static uint8 type = 0;
    static uint value = 0;

    if (step == 0)
    {
    }
    else if (step == 1)
    {
        type = data;
    }
    else
    {
        value >>= 8;
        value &= 0x00ffffff;
        value |= (data << 24);

        if (step == 5)
        {
            int index = BackBuffer::Signal::GetColorIndex(type & 0x01, (type >> 1) & 1);
            COLOR(index) = value;
            BackBuffer::LoadPalette();
            return true;
        }
    }

    return false;
}


void PDecoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}


bool PDecoder::DrawSignal(uint8 data)
{
    volatile static uint8 mode;
    static Point2 left_top;
    static Point2 left_bottom;
    static int num_points;

    static int x0;
    static int y_top;
    volatile static int y_bottom;

    if (step > 0)
    {
        if (step == 1)
        {
            mode = data;
            BackBuffer::Signal::SetChannel((mode >> 2) & 1);
        }
        else if (step < 5)
        {
            left_top.Append(data);

            if (step == 4)
            {
                x0 = left_top.X();
                y_top = left_top.Y();
            }
        }
        else if (step < 8)
        {
            left_bottom.Append(data);

            if (step == 7)
            {
                y_bottom = left_bottom.Y();
            }
        }
        else if (step == 8)
        {
            num_points = data;
        }
        else if (step == 9)
        {
            num_points += data << 8;

            Converter::Prepare(y_bottom, y_top);
        }
        else
        {
            int current_number = step - 10;         // Номер текущей точки

            static uint8 prev_adc = 0;              // Значение предыдущей точки

            if (current_number == 0)
            {
                prev_adc = data;
            }
            else
            {
                if (mode == 0)                              // пик дет откл, точки
                {
                    BackBuffer::Signal::DrawPoint(x0++, Converter::CacluateY(data));
                }
                else if (mode == 1)                         // пик дет вкл, точки
                {

                }
                else if (mode == 2)                         // пик дет откл, линии
                {
                    int y1 = Converter::CacluateY(prev_adc);
                    int y2 = Converter::CacluateY(data);

                    BackBuffer::DrawLine(x0, y1, x0 + 1, y2);

                    x0++;

                    prev_adc = data;
                }
                else if (mode == 3)                         // пик дет вкл, линии
                {

                }
            }

            if (current_number == num_points - 1)
            {
                left_top.Reset();
                left_bottom.Reset();

                return true;
            }
        }
    }

    return false;
}
