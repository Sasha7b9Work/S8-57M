// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Utils/String.h"


#define SIZE_BUFFER 100


void Log::Message(pchar message)
{
    Console::AddString(message);
}


void Log::Message(pchar file, int line, pchar message)
{
    Console::AddString(String("%s %d %s", file, line, message).c_str());
}
