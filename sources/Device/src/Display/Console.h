// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Console
{
    void Init();

    void Render();

    void AddString(const char *string);
    // ���������� true, ���� ������� ������ ������������ �� ������
    bool IsShown();
    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    void OnChanged_MaxStringsInConsole();
};

