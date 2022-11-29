#pragma once



struct Key
{
    enum E
    {
        None,
        Function,
        Measure,
        Memory,
        Service,
        ChannelA,
        ChannelB,
        Time,
        Start,
        Trig,
        Display,
        RangeMoreA,
        RangeLessA,
        RShiftMoreA,
        RShiftLessA,
        RangeMoreB,
        RangeLessB,
        RShiftMoreB,
        RShiftLessB,
        TBaseMore,
        TBaseLess,
        TShiftMore,
        TShiftLess,
        TrigLevMore,
        TrigLevLess,
        Left,
        Right,
        Up,
        Down,
        Enter,
        F1,
        F2,
        F3,
        F4,
        F5,
        Count
    } value;

    explicit Key(E v = None) : value(v) {};
    
    bool IsFunctional() const { return value >= F1 && value <= F5; };
    
    bool IsArrow() const { return value >= Left && value <= Down; };
    
    // ���������� true, ���� ������ ��������� ���������� ������� - ��������� ���, ���������
    bool IsControlSignal() const;
    
    pString Name();
};



struct TypePress
{
    enum E
    {
        Press,      // ������� ������
        Repeat,     // �������������� ������
        Release,    // ���������� ������
        Long,       // "�������" ������� ������
        None,
        Count
    } value;

    explicit TypePress(E v) : value(v)  { };
    
    pString ToString();
    
    bool IsLong() const { return value == Long; };
    
    const char *ToChar() const;
};



struct KeyEvent
{
    explicit KeyEvent(Key::E k = Key::None, TypePress::E t = TypePress::None) : key(k), type(t) { };
    
    Key::E key;
    TypePress::E type;
    
    // ���������� true, ���� ������ �������� ����������
    bool IsIncrease() const { return key == Key::Up || key == Key::Right; }
    
    bool IsDecrease() const { return key == Key::Down || key == Key::Left; }
    
    // ���������� 1, ���� ������ �������� ����������, � -1 � �������� ������
    int Delta() const { return IsIncrease() ? 1 : -1; }
    
    bool IsArrowLeft() const { return key == Key::Left; };
    
    bool IsArrowRight() const { return key == Key::Right; };
    
    bool IsArrowUp() const { return key == Key::Up; };
    
    bool IsArrowDown() const { return key == Key::Down; };
    
    bool IsArrow() const;
    
    bool IsPress() const { return type == TypePress::Press; };
    
    bool IsRepeat() const { return type == TypePress::Repeat; };
    
    bool IsLong() const { return type == TypePress::Long; }
    
    bool IsRelease() const { return type == TypePress::Release; }
};



struct Keyboard
{
    // ������������� ����������. ����� ��������� ������ ������ �� ������� keys, ���������������� ����
    static void Lock(const Key::E * const keys);
    
    // �������������� ����������
    static void Unlock();
    
    // ���������� true, ���� ������ �� �������������
    static bool KeyIsActive(Key::E key);

    static void ShortPress(Key::E key);
};
