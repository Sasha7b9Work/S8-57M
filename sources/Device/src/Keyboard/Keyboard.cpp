#include "defines.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"


// ��������� �� ������ ������, ������� ��������� ��� ���������. ���� == 0, �� ��������� ��� ������
static const Key::E *allowedKeys = 0;


pString Key::Name()
{
    static const pString names[Key::Count] =
    {
        "None",
        "�������",
        "�����",
        "������",
        "������",
        "����� 1",
        "����� 2",
        "���������",
        "����/����",
        "�����",
        "�������",
        "Range 1 ������",
        "Range 1 ������",
        "RShift 1 ������",
        "RShift 1 ������",
        "Range 2 ������",
        "Range 2 ������",
        "RShift 2 ������",
        "RShift 2 ������",
        "TBase ������",
        "TBase ������",
        "TShift ������",
        "TShift ������",
        "����� ������",
        "����� ������",
        "�����",
        "������",
        "�����",
        "����",
        "����",
        "1",
        "2",
        "3",
        "4",
        "5"
    };

    return names[value];
}


pString TypePress::ToString()
{
    static const pString names[TypePress::Count] =
    {
        "����",
        "������",
        "�����",
        "�������",
        "�����"
    };

    return names[value];
}


bool Key::IsControlSignal() const
{
    static const bool isControl[Key::Count] =
    {
        false, // None
        false, // Function
        false, // Measure
        false, // Memory
        false, // Service
        false, // ChannelA
        false, // ChannelB
        false, // Time
        false, // Start
        false, // Trig
        false, // Display
        true,  // RangeMoreA
        true,  // RangeLessA
        true,  // RShiftMoreA
        true,  // RShiftLessA
        true,  // RangeMoreB
        true,  // RangeLessB
        true,  // RShiftMoreB
        true,  // RShiftLessB
        false, // TBaseMore
        false, // TBaseLess
        false, // TShiftMore
        false, // TShiftLess
        false, // TrigLevMore
        false, // TrigLevLess
        false, // Left
        false, // Right
        false, // Up
        false, // Down
        false, // Enter
        false, // F1
        false, // F2
        false, // F3
        false, // F4
        false  // F5
    };


    return  isControl[value];
}


bool KeyEvent::IsArrow() const
{
    return (key == Key::Up) || (key == Key::Down) || (key == Key::Left) || (key == Key::Right);
}



void Keyboard::Lock(const Key::E *const keys)
{
    allowedKeys = keys;
}


void Keyboard::Unlock()
{
    allowedKeys = 0;
}


bool Keyboard::KeyIsActive(Key::E key)
{
    if (allowedKeys == 0)
    {
        return true;
    }

    const Key::E *nextKey = allowedKeys;

    while (nextKey)
    {
        if (*nextKey == 0)
        {
            break;
        }

        if (*nextKey == key)
        {
            return true;
        }
        nextKey++;
    }

    return false;
}


const char *TypePress::ToChar() const
{
    static const char *ugo[TypePress::Count] =
    {
        "+",
        "������",
        "-",
        "���������"
        "������ �� ������"
    };

    return ugo[value];
}


void Keyboard::ShortPress(Key::E key)
{
    BufferButtons::Push(KeyEvent(key, TypePress::Press));
    BufferButtons::Push(KeyEvent(key, TypePress::Release));

    while (!BufferButtons::IsEmpty())
    {
        Menu::Update();
    }
}
