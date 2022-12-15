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

    static bool DrawSignal(uint8);

    // Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();
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
        E,
        DrawSignal
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
    if (step == 1)
    {
        BackBuffer::BeginPaint((int)data);
    }
    return true;
}


bool PDecoder::DisplayBrightness(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        HAL_DAC2::SetValue(data);
    }

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
    if (step == 1)
    {
        Color(data).SetAsCurrent();
    }
    return true;
}


bool PDecoder::FuncScreen(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        BackBuffer::SendRow(data);
    }

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
        if (Resolution::IsFull())
        {
            BackBuffer::FillRegion(pos.X(), pos.Y(), size.Width(), size.Height());
        }
        else
        {
            BackBuffer::FillRegion(pos.X() * 2, pos.Y() * 2, size.Width() * 2, size.Height() * 2);
        }
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
        if (Resolution::IsFull())
        {
            BackBuffer::DrawRectangle(pos.X(), pos.Y(), size.Width(), size.Height());
        }
        else
        {
            BackBuffer::DrawRectangle(pos.X() * 2, pos.Y() * 2, size.Width() * 2, size.Height() * 2);
            BackBuffer::DrawRectangle(pos.X() * 2 + 1, pos.Y() * 2 + 1, size.Width() * 2 - 2, size.Height() * 2 - 2);
        }
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
                if (Resolution::IsFull())
                {
                    BackBuffer::DrawVLine(pos1.X(), pos1.Y(), pos2.Y());
                }
                else
                {
                    BackBuffer::DrawVLine(pos1.X() * 2, pos1.Y() * 2, pos2.Y() * 2);
                    BackBuffer::DrawVLine(pos1.X() * 2 + 1, pos1.Y() * 2, pos2.Y() * 2);
                }
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
                if (Resolution::IsFull())
                {
                    BackBuffer::DrawHLine(pos1.Y(), pos1.X(), pos2.X());
                }
                else
                {
                    BackBuffer::DrawHLine(pos1.Y() * 2, pos1.X() * 2, pos2.X() * 2);
                    BackBuffer::DrawHLine(pos1.Y() * 2 + 1, pos1.X() * 2, pos2.X() * 2);
                }
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
        if (Resolution::IsFull())
        {
            BackBuffer::DrawVPointLine(pos.X(), pos.Y(), delta, count);
        }
        else
        {
            BackBuffer::DrawVPointLine(pos.X() * 2, pos.Y() * 2, delta * 2, count);
            BackBuffer::DrawVPointLine(pos.X() * 2 + 1, pos.Y() * 2, delta * 2, count);
        }
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
        if (Resolution::IsFull())
        {
            BackBuffer::DrawHPointLine(pos.X(), pos.Y(), delta, count);
        }
        else
        {
            BackBuffer::DrawHPointLine(pos.X() * 2, pos.Y() * 2, delta * 2, count);
            BackBuffer::DrawHPointLine(pos.X() * 2, pos.Y() * 2 + 1, delta * 2, count);
        }
        pos.Reset();
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


bool PDecoder::SetMinWidthFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Text::SetMinWidthFont(data);
    }

    return true;
}


bool PDecoder::SetTextSpacing(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Text::SetSpacing(data);
    }

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
        pos0.Append(data);
        break;
    case 4:
    case 5:
        pos1.Append(data);
        break;
    case 6:
        pos1.Append(data);
        if (Resolution::IsFull())
        {
            BackBuffer::DrawLine(pos0.X(), pos0.Y(), pos1.X(), pos1.Y());
        }
        else
        {
            BackBuffer::DrawLine(pos0.X() * 2, pos0.Y() * 2, pos1.X() * 2, pos1.Y() * 2);
            BackBuffer::DrawLine(pos0.X() * 2 + 1, pos0.Y() * 2, pos1.X() * 2 + 1, pos1.Y() * 2);
        }
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
            if (Resolution::IsFull())
            {
                BackBuffer::FillRegion(pos.X(), pos.Y(), 1, 1);
            }
            else
            {
                BackBuffer::FillRegion(pos.X() * 2, pos.Y() * 2, 1, 1);
            }

            pos.Reset();

            return true;
        }
    }

    return false;
}


bool PDecoder::DrawSignal(uint8 data)
{
    static uint8 mode;
    static Point2 left_top;
    static Point2 left_bottom;
    static int num_points;

    if (step > 0)
    {
        if (step == 1)
        {
            mode = data;
        }
        else if (step < 5)
        {
            left_top.Append(data);
        }
        else if (step < 8)
        {
            left_bottom.Append(data);
        }
        else if (step == 8)
        {
            num_points = data;
        }
        else if (step == 9)
        {
            num_points += data << 8;
        }
        else
        {

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
        buffer = new char[static_cast<uint>(numSymbols + 1)]; //-V2511
        break;
    default:
        buffer[readingSymbols++] = static_cast<char>(data);
        if (readingSymbols == numSymbols)
        {
            buffer[readingSymbols] = 0;
            if (Resolution::IsFull())
            {
                Text::Draw(pos.X(), pos.Y(), buffer, 1);
            }
            else
            {
                Text::Draw(pos.X() * 2, pos.Y() * 2, buffer, 2);
            }
            pos.Reset();
            delete[]buffer; //-V2511
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
    case 3: valueColor |= static_cast<uint>(data) << 8;     break;
    case 4: valueColor |= static_cast<uint>(data) << 16;    break;
    case 5: valueColor |= static_cast<uint>(data) << 24;
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

    PFont::Set(static_cast<PTypeFont::E>(data));

    return true;
}


void PDecoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
