#include "defines.h"
#include "Timer.h"
#include <stm32f4xx.h>
#include "stm32/Timer4XX.h"
#include <limits.h>
#include "Hardware/CPU.h"


#if defined(STM32F437xx) || defined(STM32F407xx) || defined(STM32F429xx)

static Timer4XX tim2;   // ��� �����
static Timer4XX tim3;   // ��� ��������

#elif defined STM32F207xx

static Timer2XX tim2;   // ��� �����
static Timer2XX tim3;   // ��� ��������

#endif


typedef struct
{
    pFuncVV func;       // ������� �������
    uint    dTms;          // ������ ������������, ��
    uint    timeNextMS;    // ����� ���������� ������������. ���� == 0xffffffff, �� ������ ���������
    bool    repeat;        // ���� true, ����� �����������, ���� �� ����� ������� ������� Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
} TimerStruct;



static TimerStruct timers[NumTimers];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;



#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)



/// ������� �����, ������� ����������� � timeStop ��
static void StartTIM(uint timeStop);

static void StopTIM();
/// ���������� ����� ������������ ���������� �������, ���� 0, ���� �������� ���
static uint NearestTime();
/// ��������� ������� �� ������
static void TuneTIM(TypeTimer type);
/// ���������� ��� ������������ �������
static void ElapsedCallback();



bool Timer::IsRun(TypeTimer type)
{
    return TIME_NEXT(type) != UINT_MAX;
}


void Timer::Init()
{
    for(uint i = 0; i < NumTimers; i++)
    {
        timers[i].timeNextMS = UINT_MAX;
    }
   
    tim3.Init(TIM3, 54000 - 1, TIM_COUNTERMODE_UP, 1, TIM_CLOCKDIVISION_DIV1); //-V2571
    tim3.EnabledIRQ(1, 1);

    tim2.Init(TIM2, 0, TIM_COUNTERMODE_UP, static_cast<uint>(-1), TIM_CLOCKDIVISION_DIV1); //-V2571
    tim2.Start();
}


void Timer::DeInit()
{
    tim2.Stop();
    tim2.DeInit();
    
    tim3.DisableIRQ();
    tim3.StopIT();
    tim3.DeInit();
}


static void ElapsedCallback() //-V2506
{
    uint time = TIME_MS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < NumTimers; type++)
    {
        if (TIME_NEXT(type) <= time)            // ���� ������ ����� ������������
        {
            TimerStruct *timer = &timers[type];
            timer->func();
            if (timer->repeat)
            {
                do      // ���� ����� ������, ��� ��������� ������ SysTick, ������� ����������� ������������, ����� ��������� ���������,
                {       // � ���� ������� ����������� ������, ��� timer->dTm ��, �� ��� ��� ��������
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < TIME_MS);

            }
            else
            {
                timer->timeNextMS = UINT_MAX;
            }
        }
    }

    StartTIM(NearestTime());
}


void Timer::Set(TypeTimer type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}


void Timer::SetAndStartOnce(TypeTimer type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}


void Timer::SetAndEnable(TypeTimer type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}


void Timer::StartOnce(TypeTimer type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}


void Timer::Enable(TypeTimer type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}


static void TuneTIM(TypeTimer type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = TIME_MS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // ���� ������ ������ ��������� ������ ��������
    {
        StartTIM(timeNext);         // �� ������� ������ �� ���� �����
    }
}


void Timer::Disable(TypeTimer type)
{
    timers[type].timeNextMS = UINT_MAX;
    timers[type].repeat = false;
}


static uint NearestTime()
{
    uint time = UINT_MAX;

    for(uint type = 0; type < NumTimers; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}


static void StartTIM(uint timeStopMS) //-V2506
{
    StopTIM();

    if(timeStopMS == UINT_MAX)
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    tim3.StartIT((dT * 2) - 1);             // 10 ������������� 0.1��. �.�. ���� ��� ����� 1��, ����� �������� (100 - 1)
}


static void StopTIM()
{
    tim3.StopIT();
}


void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}


void Timer::PauseOnOPS(uint ops)
{
    volatile uint count = 0;
    for (; count < ops; count++)
    {
    }
}


void Timer::PauseOnTicks(uint numTicks)
{
    uint startTicks = TIME_TICKS; //-V2571
    while (TIME_TICKS - startTicks < numTicks) //-V2571
    {
    };
}


void Timer::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN; //-V2571
    TIM2->CNT = 0; //-V2571
    TIM2->CR1 |= TIM_CR1_CEN; //-V2571
}


void Timer::StartLogging()
{
    timeStartLogging = TIME_TICKS; //-V2571
    timePrevPoint = timeStartLogging;
}


uint Timer::LogPointUS(char *)
{
    uint interval = TIME_TICKS - timePrevPoint; //-V2571
    timePrevPoint = TIME_TICKS; //-V2571
    return interval;
}

#ifdef __cplusplus
extern "C" {
#endif

    //---------------------------
    void TIM3_IRQHandler()
    {
        if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF) //-V2571
        {
            if((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE) //-V2571
            {
                TIM3->SR = ~TIM_DIER_UIE; //-V2571
                ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif


#undef TIME_NEXT
