#pragma once


typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void AddData(uint data);

    // Возвращает true, если была обработка
    bool Update();

    // Возвращает размер буфера, ожидающего обработки
    int BytesInBuffer();

    // Возвращает указатель на первый элемент необработанных данных
    uint8 *Buffer();
};
