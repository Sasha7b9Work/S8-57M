// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
     ласс единственно предназначен дл€ автоматического выделени€/освобождени€ пам€ти из кучи
*/

class Buffer
{
public:
    Buffer(int size = 0);
    ~Buffer();
    void Realloc(int size);
    void Fill(uint8 value);
    uint8 *data;
    int Size() const { return size; }
    char *DataChar() { return reinterpret_cast<char *>(data); }

    void Log();
    static void LogU8_10(const uint8 *buffer, int num_point = 0);
    // ¬ыводит разность между максимальным и минимальным элементами
    static void DeltaU8_10(const uint8 *buffer, int num_point = 0);
    static uint8 MinU8_10(const uint8 *buffer);
    static uint8 MaxU8_10(const uint8 *buffer);
private:
    int size;
    void Free();
    void Malloc(int size);
};
