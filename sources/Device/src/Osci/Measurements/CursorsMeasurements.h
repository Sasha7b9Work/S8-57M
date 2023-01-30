// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CursorsMeasurements
{
    // Нарисовать курсоры и результаты курсорных измерений
    void Draw();

    // Возвращает true,если нужно рисовать курсоры
    bool  NecessaryDraw();

    // Получить позицию курсора напряжения
    float PosU(Ch::E, int numCur);

    // Возвращает значение курсора времени
    float PosT(Ch::E, int num);

    // Получить строку курсора напряжения
    String Voltage(Ch::E source, int numCur);

    // Получть строку курсора времени
    String Time(Ch::E source, int numCur);

    // Возвращает строку процентных измерений по напряжению
    String PercentsU(Ch::E source);

    // Возвращает строку процентных измерений по времени
    String PercentsT(Ch::E source);

    void SetCursPosT_temp(Ch::E, int num, float value);
};
