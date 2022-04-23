#include "defines.h"
#ifdef DEVICE
#include "Menu/Menu.h"
#endif
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"
#include "common/Communicator/Message_.h"
#include "common/Communicator/Communicator_.h"
#include <cstdlib>


namespace DDecoder
{
    int pointer = 0;
    pFuncBU8 curFunc = nullptr;  // Выполняемая функция
    int step = 0;                // Текущий байт выполняемой функции
    const int SIZE_BUFFER = 512;
    uint8 buffer[SIZE_BUFFER];

    bool E(uint8);

    void RunStep(uint8 data);

    // Эту функцию надо вызывать после выполнения последнего шага
    void FinishCommand();

    // Добавляет текстовую строку в консоль
    bool AddToConsole(uint8);

    bool ButtonPress(uint8);
}


void DDecoder::AddData(uint data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = (uint8)data;
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


bool DDecoder::E(uint8)
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
        E,              // None,
        ButtonPress,    // ButtonPress,
        E,              // Paint_DrawBigText,
        AddToConsole,   // AddToConsole
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
        return false;
    }

    if (step == 1)
    {
        allSymbols = data;
        text = static_cast<char *>(std::malloc(allSymbols + 1U));
        recvSymbols = 0;
        return false;
    }

    text[recvSymbols++] = static_cast<char>(data);

    if (recvSymbols == allSymbols)
    {
        std::free(text);
    }

    return (recvSymbols == allSymbols);
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
