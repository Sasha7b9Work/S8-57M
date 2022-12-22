// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Test
{
    void Run();

    namespace Display
    {
        void Init();

        void DeInit();

        void StartTest(pchar nameTest);

        int AddMessage(pchar message, int num = -1);
    }

    namespace ROM
    {
        namespace Data
        {
            bool Test();
        }
    }

    namespace RAM
    {
        bool Test();

    }
}
