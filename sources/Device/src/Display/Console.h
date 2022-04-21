#pragma once


namespace Console
{
    void Init();

    void Draw();

    void AddString(const char* string);

    // ���������� true, ���� ������� ������ ������������ �� ������
    bool IsShown();

    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    void OnChanged_MaxStringsInConsole();
};
