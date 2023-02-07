// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Stack.h"
#include <cstdlib>


template class Stack<uint>;
template class Stack<int8>;
template class Stack<uint8>;
template Stack<uint>::Stack(int);
template Stack<uint8>::Stack(int);
template Stack<int8>::Stack(int);
template Stack<uint>::~Stack();
template Stack<uint8>::~Stack();
template Stack<int8>::~Stack();
template uint Stack<uint>::Pop();
template uint8 Stack<uint8>::Pop();
template int8 Stack<int8>::Pop();
template void Stack<uint>::Push(uint);
template void Stack<uint8>::Push(uint8);
template void Stack<int8>::Push(int8);
template int Stack<uint>::NumFirstZeros() const;
template int Stack<uint>::Size() const;
template int Stack<int8>::Size() const;
template bool Stack<uint>::Empty() const;


template <typename T>
Stack<T>::Stack(int _size) : buffer(0), size(_size), numElements(0U)
{
    buffer = (T *)std::malloc(sizeof(T) * _size);
}


template <typename T>
Stack<T>::~Stack()
{
    std::free(buffer);
}


template <typename T>
void Stack<T>::Push(T elem)
{
    if(numElements < size)
    {
        buffer[numElements] = elem;
        numElements++;
    }
}


template <typename T>
T Stack<T>::Pop()
{
    if(numElements != 0)
    {
        numElements--;
        return buffer[numElements];
    }

    return (T)0;
}


template <typename T>
int Stack<T>::Size() const
{
    return numElements;
}


template <typename T>
bool Stack<T>::Empty() const
{
    return Size() == 0;
}


template <typename T>
int Stack<T>::NumFirstZeros() const
{
    int result = 0;

    for(int i = 0; i < size; i++)
    {
        if(buffer[i] != 0)
        {
            break;
        }
        result++;
    }

    return result;
}
