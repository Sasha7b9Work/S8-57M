// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "structs.h"
#include "Decoder_p.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"



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

// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();


void PDecoder::AddData(uint8 data) //-V2506
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
        E,
        DisplayBrightness,
        FuncScreen,
        DrawVPointLine,
        DrawHPointLine,
        SetMinWidthFont,
        SetTextSpacing,
        E
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


static bool InButtonPress(uint8) //-V2506
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


static bool BeginScene(uint8)
{
    Display::ClearBuffer();
    return true;
}


static bool DisplayBrightness(uint8 data) //-V2506
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


static bool EndScene(uint8)
{
    Painter::EndScene();
    return true;
}


static bool SetColor(uint8 data) //-V2506
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Painter::SetColor(static_cast<Color>(data));
    }
    return true;
}


static bool FuncScreen(uint8 data) //-V2506
{
    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        Painter::SendRow(data);
    }

    return true;
}


static bool FillRegion(uint8 data)
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
            Painter::FillRegion(pos.X() * 2, pos.Y() * 2, size.Width() * 2, size.Height() * 2);
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


static bool DrawRectangle(uint8 data)
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
            Painter::DrawRectangle(pos.X() * 2, pos.Y() * 2, size.Width() * 2, size.Height() * 2);
            Painter::DrawRectangle(pos.X() * 2 + 1, pos.Y() * 2 + 1, size.Width() * 2 - 2, size.Height() * 2 - 2);
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


static bool DrawVLine(uint8 data)
{
    static Point2 pos1;
    static Point2 pos2;
    
    bool result = false;

    switch (step)
    {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            pos1.Append(data);
            break;
        case 4:
        case 5:
            pos2.Append(data);
            break;
        case 6:
            pos2.Append(data);
            Painter::DrawVLine(pos1.X() * 2, pos1.Y() * 2, pos2.Y() * 2);
            Painter::DrawVLine(pos1.X() * 2 + 1, pos1.Y() * 2, pos2.Y() * 2);
            pos1.Reset();
            pos2.Reset();
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}


static bool DrawHLine(uint8 data)
{
    static Point2 pos1;
    static Point2 pos2;

    bool result = false;

    switch (step)
    {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            pos1.Append(data);
            break;
        case 4:
        case 5:
            pos2.Append(data);
            break;
        case 6:
            pos2.Append(data);
            Painter::DrawHLine(pos1.Y() * 2, pos1.X() * 2, pos2.X() * 2);
            Painter::DrawHLine(pos1.Y() * 2 + 1, pos1.X() * 2, pos2.X() * 2);
            pos1.Reset();
            pos2.Reset();
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}


static bool DrawVPointLine(uint8 data)
{
    static int y;
    static int x;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:                                     break;
    case 1: x = data;                           break;
    case 2: x += static_cast<int>(data) << 8;   break;
    case 3: y = data;                           break;
    case 4: delta = data;                       break;
    case 5: count = data;
        Painter::DrawVPointLine(x * 2, y * 2, delta * 2, count);
        Painter::DrawVPointLine(x * 2 + 1, y * 2, delta * 2, count);
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


static bool DrawHPointLine(uint8 data)
{
    static int y;
    static int x;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:                                     break;
    case 1: x = data;                           break;
    case 2: x += static_cast<int>(data) << 8;   break;
    case 3: y = data;                           break;
    case 4: delta = data;                       break;
    case 5: count = data;
        Painter::DrawHPointLine(x * 2, y * 2, delta * 2, count);
        Painter::DrawHPointLine(x * 2, y * 2 + 1, delta * 2, count);
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}


static bool SetMinWidthFont(uint8 data) //-V2506
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


static bool SetTextSpacing(uint8 data) //-V2506
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


static bool DrawLine(uint8 data)
{
    __IO static int x0;
    __IO static int y0;
    __IO static int x1;
    
    bool result = false;

    switch (step)
    {
        case 0:                                     break;
        case 1: x0 = data;                          break;
        case 2: x0 += static_cast<int>(data) << 8;  break;
        case 3: y0 = data;                          break;
        case 4: x1 = data;                          break;
        case 5: x1 += static_cast<int>(data) << 8;  break;
        case 6: // Painter::DrawLine(x0, y0, x1, data);
            Painter::DrawLine(x0 * 2, y0 * 2, x1 * 2, data * 2);
            Painter::DrawLine(x0 * 2 + 1, y0 * 2, x1 * 2 + 1, data * 2);
            result = true;
            break;
        default:
            result = true;
            break;
    }

    return result;
}


static bool SetPoint(uint8 data)
{
    static int x = 0;
    
    bool result = false;

    switch (step)
    {
        case 0:                                     break;
        case 1: x = data;                           break;
        case 2: x += static_cast<int>(data) << 8;   break;
        case 3: // Painter::SetPoint(x, data);
            Painter::FillRegion(x * 2, data * 2, 1, 1);
            result = true;
            break;
        default:
            result = true;
            break;
    }

    return result;
}


static bool DrawText(uint8 data) //-V2506
{
    static Point2 pos;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
        case 0:                         break;
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
                Text::Draw(pos.X() * 2, pos.Y() * 2, buffer, 2);
                pos.Reset();
                delete []buffer; //-V2511
                return true;
            }
            break;
    }
    return false;
}


static bool SetPalette(uint8 data)
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
            Painter::SetColorValue(Color(numColor), valueColor);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}


static bool SetFont(uint8 data) //-V2506
{
    if (step == 0)
    {
        return false;
    }

    PFont::Set(static_cast<PTypeFont::E>(data));

    return true;
}


static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
