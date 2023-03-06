// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


// Здесь хранятся настройки, которые определяются при калибровке и не перезаписываются при выключении прибора


#define NRST_NUM_SMOOTH_FOR_RAND    (setNRST.numSmoothForRand)
#define NRST_CORRECTION_TIME        (setNRST.correctionTime)
#define NRST_ENUM_GATE_MAX          (setNRST.enumGateMax)
#define NRST_ENUM_GATE_MIN          (setNRST.enumGateMin)
#define NRST_EX_SHIFT(chan, range)  (setNRST.exShift[chan][range])
#define NRST_EX_STRETCH(chan)       (setNRST.exStretch[chan])
#define NRST_ENUM_AVERAGE_RAND      (setNRST.enumAverageRand)


struct SettingsNRST
{ //-V802
    /*
        ВНИМАНИЕ !!! Для корректной работы нельзя уменьшать размер этой структуры
    */

    uint   size;                                    // Размер структуры. За одно при загрузке из памяти можно определить, что структура пуста - здесь будет значение 0xFFFFFFFF
    int16  numSmoothForRand;                       // Число точек для скользящего фильта в рандомизаторе.
    int16  correctionTime;                         // Коэффициент коррекции времени.
    int16  enumGateMax;                            // Ограничение ворот в рандомизаторе сверху
    int16  enumGateMin;                            // Ограничение ворот в рандомизаторе снизу
    int8   exShift[Ch::Count][Range::Count];     // Дополнительное смещение
    float  exStretch[Ch::Count];                 // Дополнительная растяжка по каналу
    int8   enumAverageRand;                        // Количество дополнительных усреднений в рандомизаторе
    // Инициализация при включении. Проиходит определние наличия настроек в ROM и их загрузка в случае, если настройки есть. Если настроек нету - инициализация значениями по умолчанию
    void Init();
    // Сохранение настроек в ROM. Происходит только если настройки в setNRST не совпадают с хранящимися в ROM
    void Save();

    void Reset();

    bool operator!=(const SettingsNRST &rhs);

    // Сбросить дополнительное смеещение АЦП
    void SetExtraShift(int8 shift);

    // Сбросить дополнительную растяжку АЦП
    void ResetExtraStretch();
};


extern SettingsNRST setNRST;
