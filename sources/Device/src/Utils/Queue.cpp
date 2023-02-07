// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Utils/Queue.h"


template class Queue<WarningStruct>;
template Queue<WarningStruct>::Queue();
template Queue<String *>::Queue();
template Queue<WarningStruct>::~Queue();
template Queue<String *>::~Queue();
template WarningStruct &Queue<WarningStruct>::operator[](int);
template String *&Queue<String *>::operator[](int);
template WarningStruct Queue<WarningStruct>::Back();
template String* Queue<String *>::Back();
template WarningStruct Queue<WarningStruct>::Front();
template int Queue<WarningStruct>::Size() const;
template int Queue<String *>::Size() const;
template bool Queue<WarningStruct>::IsEmpty() const;
template void Queue<WarningStruct>::Push(WarningStruct);
template void Queue<String *>::Push(String *);
template class Queue<int>;


template<typename T>
Queue<T>::Queue() : pointer(nullptr), iFront(0), iBack(0)
{

}


template<typename T>
Queue<T>::~Queue()
{
    Destroy();
}


template<typename T>
void Queue<T>::Destroy()
{
    if (pointer != nullptr)
    {
        delete[] pointer;
        pointer = nullptr;
        iFront = 0;
        iBack = 0;
    }
}


template<typename T>
void Queue<T>::Push(T elem)
{
    if (pointer == nullptr)
    {
        pointer = new T[1];
        *pointer = elem;
        iFront = 0;
        iBack = 1;
    }
    else
    {
        T *temp = pointer;
        int num = iBack - iFront + 1;
        pointer = new T[(uint)(num)];
        for (int i = 0; i < num - 1; i++)
        {
            pointer[i] = temp[i + iFront];
        }
        pointer[num - 1] = elem;
        iFront = 0;
        iBack = num;
        delete[] temp;
    }
}


template<typename T>
T Queue<T>::Front()
{
    if (pointer != nullptr)
    {
        T result = pointer[iFront];
        iFront++;
        if (iFront == iBack)
        {
            Destroy();
        }

        return result;
    }

    return T(0);
}


template<typename T>
T Queue<T>::Back()
{
    T result(0);

    if (pointer != nullptr)
    {
        result = pointer[iBack - 1];
        iBack--;
        if (iFront == iBack)
        {
            Destroy();
        }
    }

    return result;
}


template<typename T>
int Queue<T>::Size() const
{
    return (iBack - iFront);
}


template<typename T>
bool Queue<T>::IsEmpty() const
{
    return (Size() == 0);
}


template<typename T>
T &Queue<T>::operator[](int n)
{
    if (pointer != nullptr)
    {
        int index = iFront + n;
        if (index >= iFront && index < iBack)
        {
            return pointer[index];
        }
    }

	static T result(0);

    return result;
}
