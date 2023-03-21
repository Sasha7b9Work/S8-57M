// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"

#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


// Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32)
 // В одной секунде 120.000.000 тиков для С8-53 и 90.000.000 тиков для С8-54.
 // Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
#undef TIME_TICKS
#undef TIME_MS
#define TIME_TICKS HAL_TIM2::TimeTicks()
#define TIME_US    HAL_TIM2::TimeUS()
#define TIME_MS    HAL::TimeMS()


struct TypeTimer
{
    enum E
    {
        ShowLevelTrigLev,       // Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
        ShowMessages,           // Таймер для засекания время показа информационных и предупреждающих сообщений.
        MenuAutoHide,           // Таймер для отсчёта времени скрывания меню.
        RShiftMarkersAutoHide,  // Таймер для отсчёта времени скрывания дополнительных боковых меток.
        StopSound,              // Выключить звук
        StrNaviAutoHide,        // Прятать строку навигации меню
        Display,                // Таймер на ручную отрисовку экрана
        RemoveAddFunction,      // Таймер на удаление дополнительной функции отрисовки
        USB,                    ///<
        Temp,
        Count
    };
};


namespace Timer
{
    void Init();

    void DeInit();

    // Запускает счётчик для точного измерения малых отрезков времени
    void StartMultiMeasurement();

    // Фиксирует время начала фрейма
    void StartFrame();

    // Возвращает время начала фрейма
    uint TimeFrame();

    // Назначает таймеру timer функцию и время срабатывания
    void Set(TypeTimer::E type, pFuncVV func, uint dTms);

    void SetAndStartOnce(TypeTimer::E type, pFuncVV func, uint dTms);

    void SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms);

    void StartOnce(TypeTimer::E type);

    void Enable(TypeTimer::E type);

    void Disable(TypeTimer::E type);

    // Возвращает true, если таймер в данный момент ведёт отсчёт
    bool IsRunning(TypeTimer::E type);

    void PauseOnTime(uint timeMS);

    void PauseOnTicks(uint numTicks);

    // Ожидать определённое число элементарных операция в цилке for
    void PauseOnOPS(uint ops);

    bool IsBusy();

    // Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки
    void StartLogging();

    uint LogPointUS(char *name);

    uint LogPointMS(char *name);

    // Служебная функция. Вызывается строго из прерывания
    void ElapsedCallback();
};


struct TimeMeterMS
{
    TimeMeterMS()
    {
        Reset();
    }

    uint ElapsedTime() const
    {
        return TIME_MS - time_reset;
    }

    void Reset()
    {
        time_reset = TIME_MS;
    }

    void WaitMS(uint ms)
    {
        uint time_exit = TIME_MS + ms;

        while (TIME_MS < time_exit)
        {
        }
    }

private:
    uint time_reset;
};
