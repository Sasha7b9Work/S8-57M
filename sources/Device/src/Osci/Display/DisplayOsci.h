// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"
#include "Menu/Pages/Include/PageTime.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


namespace DisplayOsci
{
    void Update(int field);

    // Установить признак того, что дисплей нуждается в перерисовке
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
        // Эту функцию нужно вызывать после каждой отрисовки сигналов
        void NextFrame();

        // Сброс информации
        void Reset();
    };

    // Управление выводимой на экран частью сигнала
    namespace ShiftInMemory
    {
        // Возвращает индекс первой точки на экране
        int16 Get();

        // Устанавливает индекс первой точки на экране
        void Set(int16 shift);

        // Вызывать при изменении TPos
        void OnChangeTPos();

        // Изменить смещение в памяти на delta точек
        void Change(int delta);

        // Возвращает индекс первой точки по умолчанию при данном tPos
        int16 Default(TPos::E tPos);
    };

    namespace PainterData
    {
        void DrawData();

        // Возвращает адрес первой и последней точки на экране в координатах экрана
        BitSet64 PointsOnDisplay();

        // \brief Возращает адрес первой и последней точки в координатах экрана
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


    // Структура для обслуживания параметра, значение которого необходимо отрисовывать поверх сетки (в течение некоторого времени поисле того, как этот параметр изменился)
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

        // После изменения параметра (вращения ручки) нужно вызывать эту функцию
        void Enable(DrawingValueParameter::E v);

        void Draw();
    };
};
