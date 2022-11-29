#pragma once
#include "Keyboard/Keyboard.h"


struct ModeRedrawFM
{
    enum E
    {
        None,
        Full,
        Files,
        Folders
    };

    static void Set(E);

    static bool Is(E);

private:
    static E modeRedraw;
};


class FileManager
{
public:
    // ���������� ���� ��� ��� ����������� ����� ������
    static void Init();
    // ���������� �������� ��������
    static void Draw();
    
    static void Press_LevelUp();
    
    static void Press_LevelDown();
    
    static bool HandlerKey(const KeyEvent &event);
    
    static bool GetNameForNewFile(char name[255]);

    static void Press_Tab();
};
