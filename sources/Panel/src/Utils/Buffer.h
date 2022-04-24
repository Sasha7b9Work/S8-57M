#pragma once
#include "Log.h"
#include <cstring>


template<class T, int capacity>
class Buffer
{
public:

    Buffer() : size(capacity) {};

    Buffer(int _size) : size(capacity)
    {
        Realloc(_size);
    }

    Buffer(int _size, T value) : size(capacity)
    {
        Realloc(_size);
        Fill(value);
    }

    void Fill(T value)
    {
        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    T *First() { return buffer; }

    const T *First() const { return buffer; }

    T *Last()
    {
        return buffer + Size();
    }

    T *Pointer(int pos)
    {
        return buffer + pos;
    }

    void Realloc(int _size)
    {
        size = _size;

        if (size > capacity)
        {
            LOG_ERROR("%s:%d : Very small buffer %d, need %d", __FILE__, __LINE__, capacity, size);
            size = capacity;
        }
    }

    void ReallocAndFill(int _size, T value)
    {
        Realloc(_size);

        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    // Перевыделить память и заполнить её из buffer
    void ReallocFromBuffer(const T *in, int _size)
    {
        size = _size;

        if (size > capacity)
        {
            LOG_ERROR("%s:%d : Very small buffer %d, need %d", __FILE__, __LINE__, capacity, size);
            size = capacity;
        }

        for (int i = 0; i < size; i++)
        {
            buffer[i] = in[i];
        }
    }

    // Записать в буфер size байт из buffer. Если памяти выделено меньше, заполнить только выделенную память
    void FillFromBuffer(const T *in, int _size)
    {
        if (_size > Size())
        {
            LOG_ERROR("%s:%d : Very small buffer %d, need %d", __FILE__, __LINE__, capacity, _size);
            _size = Size();
        }

        for (int i = 0; i < _size; i++)
        {
            buffer[i] = in[i];
        }
    }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    int Empty()
    {
        return Size() == 0;
    }

    int Capacity() const
    {
        return capacity;
    }

    void Append(const T *data, int _size)
    {
        if (Size() + _size > Capacity())
        {
            LOG_ERROR("Нет места в буфере");
        }
        else
        {
            std::memcpy(&buffer[size], data, (uint)(_size * sizeof(T)));
            size += _size;
        }
    }

    void Append(T data)
    {
        if (Size() < capacity)
        {
            buffer[size++] = data;
        }
    }

    void Clear()
    {
        size = 0;
    }

    T &operator[](uint i)
    {
        if ((int)i >= 0 && (int)i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

    T &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

protected:

    int size;

    T buffer[capacity];
};


template<class T>
class Buffer1024 : public Buffer<T, 1024>
{
public:
    Buffer1024() : Buffer<T, 1024>() { };
    Buffer1024(int size) : Buffer<T, 1024>(size) { };
    Buffer1024(int size, T value) : Buffer<T, 1024>(size, value) { };
};


template<class T>
class Buffer2048 : public Buffer<T, 2048>
{
public:
    Buffer2048() : Buffer<T, 2048>() { };
    Buffer2048(int size) : Buffer<T, 2048>(size) { };
    Buffer2048(int size, T value) : Buffer<T, 2048>(size, value) { };
};
