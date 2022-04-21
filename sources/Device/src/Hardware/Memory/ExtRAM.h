#pragma once


namespace ExtRAM
{
    // Первый байт внешней памяти
    uint8* Begin();

    // За последним байтом внешней памяти
    uint8* End();

    // Записывает во внешнюю память по адресу address данные buffer, size кратен 4
    void Write(const uint8* buffer, int size, uint8* address);

    // Читает из внешней памяти address в буфер buffer данные, size кратен 4
    void Read(uint8* buffer, int size, const uint8* address);

    void Fill(uint8* begin = 0, uint8 value = 0, int size = 0);
};
