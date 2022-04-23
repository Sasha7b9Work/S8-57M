// 2022/04/20 16:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "structs.h"
#include "Display/Text.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "common/Communicator/Message_.h"
#include "common/Communicator/Communicator_.h"


typedef bool(*pFuncBU)(uint);
// Выполняемая функция
static pFuncBU curFunc;
// Текущий байт выполняемой функции
static int step = 0;


static bool E(uint) { return true; }

static bool InButtonPress(uint);

static bool DisplayBrightness(uint);

// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();


void DDecoder::AddData(uint data)
{
    static const struct StructFunc
    {
        pFuncBU func;
        StructFunc(pFuncBU f) : func(f) {};
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


static bool InButtonPress(uint)
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


static bool DisplayBrightness(uint data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        HAL_DAC2::SetValue((uint8)data);
    }

    return true;
}


static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
