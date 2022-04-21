#pragma once



namespace Utils
{
    template<typename T>
    // ���������� - ��������� ��������� �������� � ���� ��������
    class Averager
    {
    public:
        Averager(int size);
    
        ~Averager();
        
        void Push(T elem);
        
        T Value() const;

    private:
        
        T *buffer;
        
        int size;
        
        int numELements;
    };


    // �������������� ����������
    template <typename T>
    class AroundAverager
    {
    public:
        
        AroundAverager(int parts);
        
        // ����� ��������� �������� � ������������ ���������� ����������
        void Push(T elem);
        
        T Value();

    private:
        
        T value;
        
        // �� ������� ������ ������� ��������
        const int parts;
        
        // ������� �������� ����������
        int pushed;
    };
}
