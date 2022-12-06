// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Test/Test.h"
#include "Utils/Values.h"


Queue<String *> *queue = nullptr;


void Test::Display::Init()
{
    queue = new Queue<String *>(); //-V2511
}

void Test::Display::DeInit()
{
    while(queue->Size())
    {
        delete queue->Back(); //-V2511
    }

    delete queue; //-V2511
}


static void Update()
{
    for (int i = 0; i < 5; i++)
    {
        Painter::BeginScene(i, Color::BACK);

        Color::FILL.SetAsCurrent();

        int start = queue->Size() - 24;
        if (start < 0)
        {
            start = 0;
        }

        for (int i = start; i < queue->Size(); i++)
        {
            Text((*queue)[i]->c_str()).Draw(10, (i - start) * 10);
        }

        Painter::EndScene();
    }
}


static String *CreateMessage(const char *message)
{
    return new String("%3d %s", TIME_MS / 1000, message); //-V2511
}


void Test::Display::StartTest(const char *nameTest)
{
    queue->Push(CreateMessage(nameTest));

    Update();
}


int Test::Display::AddMessage(const char *message, int num)
{
    int result = num;

    if (num == -1)
    {
        queue->Push(CreateMessage(message));
        result = queue->Size() - 1;
    }
    else
    {
        delete (*queue)[num]; //-V2511
        (*queue)[num] = CreateMessage(message);
    }

    Update();

    return result;
}
