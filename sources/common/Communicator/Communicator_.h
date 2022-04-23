#pragma once


typedef bool(*pFuncBU8)(uint8);


namespace Communicator
{
    void AddData(uint data);

    namespace Device
    {
        // Возвращает true, если была обработка
        bool Update();

        // Возвращает размер буфера, ожидающего обработки
        int BytesInBuffer();

        // Возвращает указатель на первый элемент необработанных данных
        uint8 *Buffer();
    }
};
