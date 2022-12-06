// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"
#include "Menu/Pages/Include/PageTime.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


namespace DisplayOsci
{
    void Update(int field);

    // ���������� ������� ����, ��� ������� ��������� � �����������
    void SetFlagRedraw();

    namespace HiPart
    {
        void Draw();
    };

    namespace BottomPart
    {
        void Draw(int x, int y);
    };

    namespace Accumulator
    {
        // ��� ������� ����� �������� ����� ������ ��������� ��������
        void NextFrame();

        // ����� ����������
        void Reset();
    };

    // ���������� ��������� �� ����� ������ �������
    namespace ShiftInMemory
    {
        // ���������� ������ ������ ����� �� ������
        int16 Get();

        // ������������� ������ ������ ����� �� ������
        void Set(int16 shift);

        // �������� ��� ��������� TPos
        void OnChangeTPos();

        // �������� �������� � ������ �� delta �����
        void Change(int delta);

        // ���������� ������ ������ ����� �� ��������� ��� ������ tPos
        int16 Default(TPos::E tPos);
    };

    namespace PainterData
    {
        void DrawData();

        // ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
        BitSet64 PointsOnDisplay();

        // \brief ��������� ����� ������ � ��������� ����� � ����������� ������
        BitSet64 BytesOnDisplay();

        extern bool needSendToSCPI_FFT;
    };

    namespace MemoryWindow
    {
        void Draw();

        int X();

        int Y();

        int Width();

        int Height();
    };


    // ��������� ��� ������������ ���������, �������� �������� ���������� ������������ ������ ����� (� ������� ���������� ������� ������ ����, ��� ���� �������� ���������)
    namespace DrawingValueParameter
    {
        enum E
        {
            TrigLevel,
            RangeA,
            RangeB,
            RShiftA,
            RShiftB,
            TBase,
            TShift
        };

        // ����� ��������� ��������� (�������� �����) ����� �������� ��� �������
        void Enable(DrawingValueParameter::E v);

        void Draw();
    };
};
