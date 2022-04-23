// 2022/04/20 16:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "structs.h"
#include "Decoder_p.h"
#include "Display/Text.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"


typedef bool(*pFuncBU8)(uint8);
// Выполняемая функция
static pFuncBU8 curFunc;
// Текущий байт выполняемой функции
static int step = 0;


static bool E(uint8) { return true; }

static bool InButtonPress(uint8);

static bool DisplayBrightness(uint8);

// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();


void PDecoder::AddData(uint8 data)
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


static bool InButtonPress(uint8)
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


static bool DisplayBrightness(uint8 data)
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


static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
