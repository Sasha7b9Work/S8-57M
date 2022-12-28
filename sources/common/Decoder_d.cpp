// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Decoder_d.h"
#ifdef DEVICE
#include "Menu/Menu.h"
#endif
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"
#include <cstdlib>


namespace DDecoder
{
    static int pointer = 0;
    static pFuncBU8 curFunc = nullptr;  // Выполняемая функция
    static int step = 0;                // Текущий байт выполняемой функции
    static const int SIZE_BUFFER = 1024;
    static uint8 buffer[SIZE_BUFFER];

    static void RunStep(uint8 data);

    // Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();

    // Добавляет текстовую строку в консоль
    static bool AddToConsole(uint8);

    static bool ButtonPress(uint8);
}


void DDecoder::AddData(uint8 data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = data;
    }
}


bool DDecoder::Update()
{
    bool result = (pointer != 0);

    if (pointer)
    {
        for (int i = 0; i < pointer; i++)
        {
            RunStep(buffer[i]);
        }
        pointer = 0;
    }

    return result;
}


static bool E(uint8)
{
    return true;
}


void DDecoder::RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Count] =
    {
        E,
        ButtonPress,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        E,
        AddToConsole,
        E,
        E,
        E
    };

    if (step == 0)
    {
        if (data < Command::Count)
        {
            curFunc = commands[data].func;
        }
        else
        {
            FinishCommand();
            return;
        }
    }

    if (curFunc)
    {
        if (curFunc(data))
        {
            FinishCommand();
        }
        else
        {
            ++step;
        }
    }
    else
    {
        FinishCommand();
    }
}


bool DDecoder::ButtonPress(uint8 data)
{
    static Key::E button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = static_cast<Key::E>(data);
        return false;
    }
    else
    {
        BufferButtons::Push(KeyEvent(button, static_cast<TypePress::E>(data)));
    }

    return true;
}


bool DDecoder::AddToConsole(uint8 data)
{
    static char *text = nullptr;        // Здесь будет храниться принятая строка

    static uint8 allSymbols = 0;        // Количество символов в строке без учёта завершающего нуля

    static uint8 recvSymbols = 0;       // Столько символов уже принято

    if (step == 0)
    {

    }
    else if (step == 1)
    {
        allSymbols = data;
        text = (char *)std::malloc(allSymbols + 1U);
        recvSymbols = 0;
    }
    else
    {
        text[recvSymbols++] = (char)(data);

        if (recvSymbols == allSymbols)
        {
            LOG_WRITE(text);
            std::free(text);
            return true;
        }
    }

    return false;
}


void DDecoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}


int DDecoder::BytesInBuffer()
{
    return pointer;
}


uint8 *DDecoder::Buffer()
{
    return buffer;
}
