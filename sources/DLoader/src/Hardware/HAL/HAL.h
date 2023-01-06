#pragma once
#include "defines.h"


#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL_EEPROM
{
    static void EraseSector(uint startAddress);
    static void WriteBytes(uint address, const uint8 *data, int size);
};


struct HAL
{
    static void Init();

    static void ErrorHandler(pchar , int);
};


namespace HAL_BUS
{
    void Init();

    // Конфигурировать для работы по шине FSMC с альтерой и памятью
    void ConfigureToFSMC();

    namespace Panel
    {
        void SendByte(uint8 byte);
        void SendBuffer(const uint8 *data, int size);
        bool Receive();
        bool InInteraction();
    };

    struct Mode
    {
        enum E
        {
            FSMC,
            PanelRead,
            PanelWrite
        };
    };

    extern Mode::E mode;

    void InitPanel();

    // Настроить FSMC для работы с внешней RAM
    static void InitRAM();
};
