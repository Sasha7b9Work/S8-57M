// 2022/04/20 16:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Hardware/HAL/HAL.h"
#include "common/Communicator/Message_.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>



#define SIZE_BUFFER_LOG 200

static void AddToConsole(const char *text);


void Log::Write(TypeTrace type, const char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    if (type == TypeTrace_Error)
    {
        buffer[0] = 0;
        std::strcat(buffer, "!!! ERROR !!! ");
        while (*pointer++) {};
        ++pointer;
    }

    std::va_list args;
    va_start(args, format); //-V2528 //-V2567
    std::vsprintf(pointer, format, args);
    va_end(args);
    AddToConsole(buffer);
}


void Log::Trace(TypeTrace type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format); //-V2528
    std::vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    std::snprintf(numBuffer, 19, ":%d", numLine);
    message[0] = 0;

    if (type == TypeTrace_Error)
    {
        std::strcat(message, "!!!ERROR!!! ");
    }
    else if (type == TypeTrace_Info) //-V547
    {
        std::strcat(message, "            ");
    }
    else
    {
        // ������ ����� ���
    }

    std::strcat(message, module);
    std::strcat(message, " ");
    std::strcat(message, func);
    std::strcat(message, numBuffer);
    AddToConsole(message);
    AddToConsole(buffer);
}


static void AddToConsole(pchar text)
{
    DynamicMessage<1024> message(Command::AddToConsole);

    message.PushByte((uint8)std::strlen(text));

    while (*text)
    {
        message.PushByte((uint8)*text);
        text++;
    }

    message.Transmit();
}
