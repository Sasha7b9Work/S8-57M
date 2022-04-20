// 2022/04/20 16:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "structs.h"
#include "Decoder_p.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"



typedef bool(*pFuncBU8)(uint8);
// ����������� �������
static pFuncBU8 curFunc;
// ������� ���� ����������� �������
static int step = 0;



// ��������� ������� �� ����������� ������
static bool FuncScreen(uint8);

static bool E(uint8) { return true; }

static bool InButtonPress(uint8);

static bool DisplayBrightness(uint8);

// ��������� ������������� ������ ������
static bool SetMinWidthFont(uint8);
// ������������� ���������� ����� ��������� ��� ������ ������
static bool SetTextSpacing(uint8);
// ��� ������� ���� �������� ����� ���������� ���������� ����
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
        E,                  // None,
        InButtonPress,      // ButtonPress,
        DisplayBrightness,  // Paint_DrawBigText,
        FuncScreen,         // Screen
        SetMinWidthFont,    // Paint_SetMonoSpaceFont
        SetTextSpacing,     // Paint_SetTextSpacing
        E,                  // AddToConsole
        E,
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
            return;     // ����������� ������ �� ������
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


static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
