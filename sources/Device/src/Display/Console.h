#pragma once


struct Console
{
    static void Init();

    static void Draw();

    static void AddString(const char *string);
    // ���������� true, ���� ������� ������ ������������ �� ������
    static bool IsShown();
    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    static void OnChanged_MaxStringsInConsole();

private:
    static CHAR_BUF2(buffer, 33, 100);
    // true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    static bool inProcessDraw;
    // true, ���� ���������� ������� ���������� ������
    static bool inProcessAddingString;
    // ���������� ����������� ����� � �������
    static int stringInConsole;
    // ����� ����������� ���������� �������� ������������� ���������� ����� � �������
    static int16 prevMaxStrinsInConsole;

    static void DeleteFirstString();
    // ���������� true, ���� ������� ������ � � ��� ������ ����������� ������
    static bool IsBusy();
};

