// 2021/03/16 10:48:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ����� ��� ������ ������ �� ����������
class Mutex
{
public:

    Mutex() : locked(false) {}

    // ���������� � �������� ����� ����� �������� � �������, �������������� � �����������
    void Lock()
    {
        locked = true;
    };

    // ���������� � �������� ����� ����� �������� � �������, �������������� � ������������
    void Unlock()
    {
        locked = false;
    };

    // ����� ���������� ����� ���������, ����� �� �������
    bool Locked() const
    {
        return locked;
    };

private:

    bool locked;
};
