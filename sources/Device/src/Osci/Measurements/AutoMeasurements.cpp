// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Smoother.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>

static float CalculateVoltageMax(Ch::E ch);
static float CalculateVoltageMin(Ch::E ch);
static float CalculateVoltagePic(Ch::E ch);
static float CalculateVoltageMaxSteady(Ch::E ch);
static float CalculateVoltageMinSteady(Ch::E ch);
static float CalculateVoltageAmpl(Ch::E ch);
static float CalculateVoltageAverage(Ch::E ch);
static float CalculateVoltageRMS(Ch::E ch);
static float CalculateVoltageVybrosPlus(Ch::E ch);
static float CalculateVoltageVybrosMinus(Ch::E ch);
static float CalculatePeriod(Ch::E ch);
static int   CalculatePeriodAccurately(Ch::E ch); // Точно вычисляет период или целое число периодов в точках сигнала.
static float CalculateFreq(Ch::E ch);
static float CalculateTimeNarastaniya(Ch::E ch);
static float CalculateTimeSpada(Ch::E ch);
static float CalculateDurationPlus(Ch::E ch);
static float CalculateDurationMinus(Ch::E ch);
static float CalculateSkvaznostPlus(Ch::E ch);
static float CalculateSkvaznostMinus(Ch::E ch);


static float CalculateMinRel(Ch::E ch);           // Возвращает минимальное значение относительного сигнала.
static float CalculateMinSteadyRel(Ch::E ch);     // Возвращает минимальное установившееся значение относительного сигнала.
static float CalculateMaxRel(Ch::E ch);           // Возвращает максимальное значение относительного сигнала.
static float CalculateMaxSteadyRel(Ch::E ch);     // Возвращает максимальное установившееся значение относительного сигнала.
static float CalculateAverageRel(Ch::E ch);       // Возвращает среденее значение относительного сигнала.
static float CalculatePicRel(Ch::E ch);
static float CalculateDelayPlus(Ch::E ch);
static float CalculateDelayMinus(Ch::E ch);
static float CalculatePhazaPlus(Ch::E ch);
static float CalculatePhazaMinus(Ch::E ch);
// \brief Найти точку пересечения сигнала с горизонтальной линией, проведённой на уровне yLine. numItersection - порядковый номер пересечения, 
// начинается с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
static float FindIntersectionWithHorLine(Ch::E ch, int numIntersection, bool downToUp, uint8 yLine);
// Делить val1 на val2. Возвращает nan, если результат неопределён
static float Divide(float val1, float val2);

static bool isSet = false;          // Если true, то сигнал назначен.

static int firstByte = 0;
static int lastByte = 0;
// Количество байт, по которым производятся расчёты
static int nBytes = 0;


typedef float (*pFuncFCh)(Ch::E ch);
typedef char *(*pFuncPCFBPC)(float, bool, char*);

typedef struct
{
    pchar name;
    pFuncFCh    FuncCalculate;
    pFuncPCFBPC FucnConvertate;
    bool        showSign;           // Если true, нужно показывать знак.
    uint8       notUsed0[3];
} MeasureCalculate;

static char* Time2String(float, bool, char buffer[20]);
static char* Voltage2String(float, bool, char buffer[20]);
static char* Phase2String(float, bool, char buffer[20]);


static const MeasureCalculate sMeas[TypeMeasure::Count] =
{
    {"", 0, 0, false, {}},
    {"CalculateVoltageMax",         CalculateVoltageMax,         Voltage2String,                    true,  {}},
    {"CalculateVoltageMin",         CalculateVoltageMin,         Voltage2String,                    true,  {}},
    {"CalculateVoltagePic",         CalculateVoltagePic,         Voltage2String,                    false, {}},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,   Voltage2String,                    true,  {}},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,   Voltage2String,                    true,  {}},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,        Voltage2String,                    false, {}},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,     Voltage2String,                    true,  {}},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,         Voltage2String,                    false, {}},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,  Voltage2String,                    false, {}},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus, Voltage2String,                    false, {}},
    {"CalculatePeriod",             CalculatePeriod,             Time2String,                       false, {}},
    {"CalculateFreq",               CalculateFreq,               AutoMeasurements::Freq2String,     false, {}},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,    Time2String,                       false, {}},
    {"CalculateTimeSpada",          CalculateTimeSpada,          Time2String,                       false, {}},
    {"CalculateDurationPlus",       CalculateDurationPlus,       Time2String,                       false, {}},
    {"CalculateDurationPlus",       CalculateDurationMinus,      Time2String,                       false, {}},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,      AutoMeasurements::Float2String,    false, {}},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,     AutoMeasurements::Float2String,    false, {}},
    {"CalculateDelayPlus",          CalculateDelayPlus,          Time2String,                       false, {}},
    {"CalculateDelayMinus",         CalculateDelayMinus,         Time2String,                       false, {}},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,          Phase2String,                      false, {}},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,         Phase2String,                      false, {}}
};


int AutoMeasurements::markerTime[Ch::Count][2] = {{Integer::ERROR}, {Integer::ERROR}};
int AutoMeasurements::markerVoltage[Ch::Count][2] = {{Integer::ERROR}, {Integer::ERROR}};
int8 AutoMeasurements::posActive = 0;

typedef struct
{
    float value[2];
} MeasureValue;

static MeasureValue values[TypeMeasure::Count] = {{{0.0F, 0.0F}}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if(isnan(x))                return Float::ERROR;
#define EXIT_IF_ERRORS_FLOAT(x, y)  if(isnan(x) || isnan(y))    return Float::ERROR;
#define EXIT_IF_ERROR_INT(x)        if((x) == Integer::ERROR)   return Float::ERROR;

// Входной буфер данных канала ch
#define CHOICE_BUFFER (OUT(ch))


namespace AutoMeasurements
{
    // Привести смещение канала ch по вертикали к текущему
    static void CountedToCurrentRShift(Ch::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // Данные из IN_A, IN_B пересчитать к текущим настройкам и записать в OUT_A, OUT_B
    static void CountedToCurrentSettings();

    static void CountedToCurrentSettings(Ch::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // Вписать значения данных в разрещённый диапазон
    static void LimitationData(uint8 *inOut, uint numBytes);
}


void AutoMeasurements::CalculateMeasures()
{
    if(!S_MEAS_SHOW || !isSet)
    {
        return;
    }
    
    maxIsCalculating[0] = false;
    maxIsCalculating[1] = false;
    maxSteadyIsCalculating[0] = false;
    maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = false;
    minIsCalculating[1] = false;
    minSteadyIsCalculating[0] = false;
    minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = false;
    aveIsCalculating[1] = false;
    periodIsCaclulating[0] = false;
    periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = false;
    periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = false;
    picIsCalculating[1] = false;

    for(int str = 0; str < TableMeasures::NumRows(); str++)
    {
        for(int elem = 0; elem < TableMeasures::NumCols(); elem++)
        {
            Measure measure(str, elem);
            TypeMeasure::E type = measure.GetType();

            pFuncFCh func = sMeas[(int)(type)].FuncCalculate;
            if(func)
            {
                if(type == S_MEAS_MARKED || S_MEAS_MARKED == TypeMeasure::None)
                {
                    markerTime[ChanA][0] = Integer::ERROR;
                    markerTime[ChanA][1] = Integer::ERROR;
                    markerTime[ChanB][0] = Integer::ERROR;
                    markerTime[ChanB][1] = Integer::ERROR;
                    markerVoltage[ChanA][0] = Integer::ERROR;
                    markerVoltage[ChanA][1] = Integer::ERROR;
                    markerVoltage[ChanB][0] = Integer::ERROR;
                    markerVoltage[ChanB][1] = Integer::ERROR;
                }
                
                if(VIEW_MEASURES_A)
                {
                    values[(int)(type)].value[ChanA] = func(ChanA);
                }
                
                if(VIEW_MEASURES_B)
                {
                    values[(int)(type)].value[ChanB] = func(ChanB);
                }
            }
        }
    }


    // Посылаем измерение в SCPI
    if (AutoMeasuresSender::sended != TypeMeasure::Count)
    {
        TypeMeasure::E type = AutoMeasuresSender::sended;

        pFuncFCh func = sMeas[type].FuncCalculate;

        char buffer[50];

        if (func)
        {
            Measure measure(type);

            if (VIEW_MEASURES_A)    
            {
                values[type].value[ChanA] = func(ChanA);
                SCPI::SendMeasure(measure.GetStringMeasure(ChanA, buffer, 49));
            }
            if (VIEW_MEASURES_B)
            {
                values[type].value[ChanB] = func(ChanB);
                SCPI::SendMeasure(measure.GetStringMeasure(ChanB, buffer, 49));
            }
        }

        AutoMeasuresSender::sended = TypeMeasure::Count;
    }
}



float CalculateVoltageMax(Ch::E ch)
{
    float max = CalculateMaxRel(ch);
    EXIT_IF_ERROR_FLOAT(max); //-V2507
    if(S_MEAS_MARKED == TypeMeasure::VoltageMax)
    {
        Measure::SetMarkerVoltage(ch, 0, max);      // Здесь не округляем, потому что max может быть только целым
    }
    
    Range::E range = RANGE_DS(ch);

    int16 rShift = RSHIFT_DS(ch);

    uint8 value = ROUND(uint8, max);

    return VALUE::ToVoltage(value, range, rShift);
}



float CalculateVoltageMin(Ch::E ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(S_MEAS_MARKED == TypeMeasure::VoltageMin)
    {
        Measure::SetMarkerVoltage(ch, 0, min);             // Здесь не округляем, потому что min может быть только целым
    }
    
    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch),RSHIFT_DS(ch));
}



float CalculateVoltagePic(Ch::E ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(S_MEAS_MARKED == TypeMeasure::VoltagePic)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinRel(ch));
    }
    return max - min;
}



float CalculateVoltageMinSteady(Ch::E ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(S_MEAS_MARKED == TypeMeasure::VoltageMinSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, ROUND(float, min));
    }

    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageMaxSteady(Ch::E ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max); //-V2507

    if(S_MEAS_MARKED == TypeMeasure::VoltageMaxSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
    }

    return VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageVybrosPlus(Ch::E ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageVybrosPlus)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
        Measure::SetMarkerVoltage(ch, 1, maxSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, maxSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), rShift));
}



float CalculateVoltageVybrosMinus(Ch::E ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageVybrosMinus)
    {
        Measure::SetMarkerVoltage(ch, 0, min);
        Measure::SetMarkerVoltage(ch, 1, minSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, minSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), rShift));
}



float CalculateVoltageAmpl(Ch::E ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(S_MEAS_MARKED == TypeMeasure::VoltageAmpl)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxSteadyRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinSteadyRel(ch));
    }
    return max - min;
}



float CalculateVoltageAverage(Ch::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    int sum = 0;

    uint8 *data = &CHOICE_BUFFER[firstByte];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = (uint8)((float)sum / period);

    if(S_MEAS_MARKED == TypeMeasure::VoltageAverage)
    {
        Measure::SetMarkerVoltage(ch, 0, aveRel);
    }

    return VALUE::ToVoltage(aveRel, RANGE_DS(ch), RSHIFT_DS(ch));
}



float CalculateVoltageRMS(Ch::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    float rms = 0.0F;

    Range::E range = RANGE_DS(ch);
    int16 rShift = RSHIFT_DS(ch);

    uint8 *dataIn = &CHOICE_BUFFER[firstByte];

    for(int i = firstByte; i < firstByte + period; i++)
    {
        float volts = VALUE::ToVoltage(dataIn[i], range, rShift);
        rms +=  volts * volts;
    }

    rms = std::sqrtf(rms / period);

    if(S_MEAS_MARKED == TypeMeasure::VoltageRMS)
    {
        Measure::SetMarkerVoltage(ch, 0, VALUE::FromVoltage(rms, range, rShift));
    }

    return rms;
}



float CalculatePeriod(Ch::E ch)
{
    static float period[2] = {0.0F, 0.0F};

    static float firstIntersection = 0.0F;
    static float secondIntersection = 0.0F;

    if(!periodIsCaclulating[(int)(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == Uint8::ERROR)
        {
            period[(int)(ch)] = Float::ERROR;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown); //-V2507

            bool firstDownToTop = intersectionDownToTop < intersectionTopToDown;
            firstIntersection = firstDownToTop ? intersectionDownToTop : intersectionTopToDown;
            secondIntersection = FindIntersectionWithHorLine(ch, 2, firstDownToTop, (uint8)(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

            float per = TShift::ToAbs(ROUND(uint16, secondIntersection - firstIntersection), S_TIME_BASE);

            period[(int)(ch)] = per;

            periodIsCaclulating[(int)(ch)] = true;
        }
    }

    if ((S_MEAS_MARKED == TypeMeasure::Period || S_MEAS_MARKED == TypeMeasure::Freq) && periodIsCaclulating[(int)(ch)])
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    return period[(int)(ch)];
}



#define EXIT_FROM_PERIOD_ACCURACY                               \
    period[(int)(ch)] = Integer::ERROR;              \
    periodAccurateIsCalculating[(int)(ch)] = true;   \
    std::free(sums);                                            \
    return period[(int)(ch)];



int CalculatePeriodAccurately(Ch::E ch)
{
    static int period[2];

    int *sums = (int *)std::malloc((uint)(nBytes));

    if (sums == 0)
    {
        return 0;
    }

    uint8 *dataIn = CHOICE_BUFFER;

    if(!periodAccurateIsCalculating[(int)(ch)])
    {
        period[(int)(ch)] = 0;

        float pic = CalculatePicRel(ch);

        if(pic == Float::ERROR)
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = (int)(pic * 5.0F);
        sums[firstByte] = dataIn[firstByte];

        int i = firstByte + 1;
        int *pSum = &sums[i];
        uint8 *data = &dataIn[i];
        uint8 *end = &dataIn[lastByte];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < VALUE::MIN || point >= VALUE::MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *pSum = *(pSum - 1) + point;
            pSum++;
        }

        int addShift = firstByte - 1;
        int maxPeriod = (int)(nBytes * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int sum = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = nBytes - nextPeriod;

            int *pSums = &sums[firstByte + 1];

            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - sum;

                if (nextSum < sum)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[(int)(ch)] = nextPeriod;
            }
        }

        if(period[(int)(ch)] == 0)
        {
            period[(int)(ch)] = Integer::ERROR;
        }
        periodAccurateIsCalculating[(int)(ch)] = true;
    }

    std::free(sums);

    return period[(int)(ch)];
}



float CalculateFreq(Ch::E ch)
{
    float period = CalculatePeriod(ch);

    return (period == Float::ERROR) ? Float::ERROR : 1.0F / period;
}



float FindIntersectionWithHorLine(Ch::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = firstByte;
    int compValue = lastByte - 1;
    int step = PEAKDET_ENABLED(DS) ? 2 : 1;

    uint8 *data = &CHOICE_BUFFER[0];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + step] >= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + step] <= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    x -= step;

    if (num < numIntersection)
    {
        return Float::ERROR;
    }
    
    return ::Math::GetIntersectionWithHorizontalLine(x, data[x], x + step, data[x + step], yLine);
}



float CalculateDurationPlus(Ch::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(aveValue));
    }

    if (S_MEAS_MARKED == TypeMeasure::DurationPlus)
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), (TBase::E)TBASE_DS);
}



float CalculateDurationMinus(Ch::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(aveValue));
    }

    if (S_MEAS_MARKED == TypeMeasure::DurationMinus)
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculateTimeNarastaniya(Ch::E ch)   /** \todo Здесь, возможно, нужно увеличить точность - брать не целые значени расстояний между 
                                              отсчётами по времени, а рассчитывать пересечения линий. */
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(min01));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (S_MEAS_MARKED == TypeMeasure::TimeNarastaniya)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    return retValue;
}



float CalculateTimeSpada(Ch::E ch)        // \todo Аналогично времени нарастания
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(max09));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (S_MEAS_MARKED == TypeMeasure::TimeSpada)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - DisplayOsci::ShiftInMemory::Get());
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - DisplayOsci::ShiftInMemory::Get());
    }

    return retValue;
}



float CalculateSkvaznostPlus(Ch::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);
    
    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float CalculateSkvaznostMinus(Ch::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float CalculateMinSteadyRel(Ch::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    uint8 *dataIn = CHOICE_BUFFER;

    if(!minSteadyIsCalculating[(int)(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == Float::ERROR)
        {
            min[(int)(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;

            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            min[(int)(ch)] = (float)sum / numSums;

            float pic = CalculatePicRel(ch);
            if (pic == Float::ERROR)
            {
                min[(int)(ch)] = Float::ERROR;
            }
            else
            {
                int numDeleted = 0;

                int numMin = numSums;

                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                float _min = min[(int)(ch)];
                while (data <= end)
                {
                    uint8 d = *data++;

                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted; //-V127
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted; //-V127
                        }
                    }
                }
                min[(int)(ch)] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : (float)sum / numSums;
            }
        }
        minSteadyIsCalculating[(int)(ch)] = true;
    }

    return min[(int)(ch)];
}



float CalculateMaxSteadyRel(Ch::E ch)
{
    static float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[(int)(ch)])
    {
        uint8 *dataIn = CHOICE_BUFFER;

        float aveValue = CalculateAverageRel(ch);
        
        if(aveValue == Float::ERROR)
        {
            max[(int)(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            max[(int)(ch)] = (float)sum / numSums;

            float pic = CalculatePicRel(ch);

            if (pic == Float::ERROR)
            {
                max[(int)(ch)] = Float::ERROR;
            }
            else
            {
                int numDeleted = 0;

                int numMax = numSums;

                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                uint8 _max = (uint8)(max[(int)(ch)]);

                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++; //-V127
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++; //-V127
                        }
                    }
                }
                max[(int)(ch)] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : (float)sum / numSums;
            }
        }
        maxSteadyIsCalculating[(int)(ch)] = true;
    }

    return max[(int)(ch)];
}



float CalculateMaxRel(Ch::E ch)
{
    static float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[(int)(ch)])
    {
        uint8 val = Math::MaxFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        max[(int)(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        maxIsCalculating[(int)(ch)] = true;
    }

    return max[(int)(ch)];
}



float CalculateMinRel(Ch::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[(int)(ch)])
    {
        uint8 val = Math::MinFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        min[(int)(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        minIsCalculating[(int)(ch)] = true;
    }

    return min[(int)(ch)];
}



float CalculateAverageRel(Ch::E ch)
{
    static float _ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[(int)(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        _ave[(int)(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : (min + max) / 2.0F;
        aveIsCalculating[(int)(ch)] = true;
    }
    return _ave[(int)(ch)];
}



float CalculatePicRel(Ch::E ch)
{
    static float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[(int)(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[(int)(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : max - min;
        picIsCalculating[(int)(ch)] = true;
    }

    return pic[(int)(ch)];
}



float CalculateDelayPlus(Ch::E ch)
{
    float periodA = CalculatePeriod(ChanA);
    float periodB = CalculatePeriod(ChanB);

    EXIT_IF_ERRORS_FLOAT(periodA, periodB); //-V2507
    if(!Math::FloatsIsEquals(periodA, periodB, 1.05F))
    {
        return Float::ERROR;
    }

    float averageA = CalculateAverageRel(ChanA);
    float averageB = CalculateAverageRel(ChanB);

    EXIT_IF_ERRORS_FLOAT(averageA, averageB); //-V2507

    float averageFirst = (ch == ChanA) ? averageA : averageB;
    float averageSecond = (ch == ChanA) ? averageB : averageA;
    Ch::E firstChannel = ch;
    Ch::E secondChannel = (ch == ChanA) ? ChanB : ChanA;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, (uint8)(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, (uint8)(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, (uint8)(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculateDelayMinus(Ch::E ch)
{
    float period0 = CalculatePeriod(ChanA);
    float period1 = CalculatePeriod(ChanB);

    EXIT_IF_ERRORS_FLOAT(period0, period1); //-V2507

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return Float::ERROR;
    }

    float average0 = CalculateAverageRel(ChanA);
    float average1 = CalculateAverageRel(ChanB);

    EXIT_IF_ERRORS_FLOAT(average0, average1); //-V2507

    float averageFirst = (ch == ChanA) ? average0 : average1;
    float averageSecond = (ch == ChanA) ? average1 : average0;
    Ch::E firstChannel = ch;
    Ch::E secondChannel = (ch == ChanA) ? ChanB : ChanA;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, (uint8)(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, (uint8)(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, (uint8)(averageSecond));
    }

    /*
    if (MEAS_MARKED == Measure_DelayMinus)
    {
        markerTime[ch][0] = (int)((int16)firstIntersection - firstByte);
        markerTime[ch][1] = (int)((int16)secondIntersection - firstByte);
    }
    */

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float CalculatePhazaPlus(Ch::E ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if(delay == Float::ERROR || period == Float::ERROR)
    {
        return Float::ERROR;
    }
    return delay / period * 360.0F;
}



float CalculatePhazaMinus(Ch::E ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);

    float result = Float::ERROR;

    if(delay != Float::ERROR && period != Float::ERROR)
    {
        result = delay / period * 360.0F;
    }
    
    return result;
}


float Measure::CalculateCursorU(Ch::E ch, float posCurT)
{
    if(!CHOICE_BUFFER)
    {
        return 0;
    }
    
    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int rel = (int)((CHOICE_BUFFER)[(int)(points.word0) + ROUND(int, posCurT)]) - VALUE::MIN;

#define SCALE (200.0F / (VALUE::MAX - VALUE::MIN))

    float value = 200.0F - rel * SCALE;
    LIMITATION(value, 0.0F, 200.0F);
    return value;
}



float Measure::CalculateCursorT(Ch::E ch, float posCurU, int numCur)
{
    uint8 *dataIn = CHOICE_BUFFER;

    if(!dataIn)
    {
        return 0;
    }

#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)
    
    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int prevData = 200 - dataIn[FIRST_POINT] + VALUE::MIN;

    int numIntersections = 0;

    for(int i = FIRST_POINT + 1; i < LAST_POINT; i++)
    {
        int curData = 200 - (dataIn)[i] + VALUE::MIN;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }
        prevData = curData;
    }
    return 0;

#undef LAST_POINT
#undef FIRST_POINT
}



void InterpolationSinX_X(uint8 *data, int numPoints, TBase::E tBase)
{
/*
     Последовательности x в sin(x)   // Это, наверное, неправильно
2    1. 20нс : pi/2, -pi/2 ...
8    2. 10нс : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 5нс  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 2нс  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 1нс  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = {100, 50, 20, 10, 5};
    int delta = deltas[(int)(tBase)];

    uint8 *signedData = (uint8 *)(std::malloc((uint)(numPoints) / 2U));
    if (signedData == 0)
    {
        return;
    }
    int numSignedPoints = 0;
    
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            signedData[numSignedPoints++] = data[pos];
        }
    }

    // Найдём смещение первой значащей точки

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = Math::PI_F;
    float stepX0 = Math::PI_F / (float)delta;
    float x0 = Math::PI_F - stepX0;
    int num = 0;
    
    for(int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sinf(Math::PI_F / delta * (part + 1)) : std::sinf(Math::PI_F / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)                 // Здесь используем более быструю, но более неправильную арифметику целвых чисел
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = (uint8)(value);
            }
        }
    }
    
    int pos = numPoints - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift];
        pos--;
    }

    std::free(signedData);
}



String Measure::GetStringMeasure(Ch::E ch, char* buffer, int lenBuf)
{
    TypeMeasure::E type = GetType();

    if (!S_CHANNEL_ENABLED(ch))
    {
        return String("");
    }

    buffer[0] = '\0';
    std::strcpy(buffer, (ch == ChanA) ? "1: " : "2: ");

    if(!isSet || values[(int)(type)].value[(int)(ch)] == Float::ERROR)
    {
        std::strcat(buffer, "-.-");
    }
    else if(sMeas[(int)(type)].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncPCFBPC func = sMeas[(int)(type)].FucnConvertate;
        float value = values[(int)(type)].value[(int)(ch)];
       
        if ((S_DIVIDER(ch) == Divider::_10) && (func == Voltage2String))
        {
            value *= 10.0F;                         // Домножаем, если включён делитель
        }
               
        char *text = func(value, sMeas[(int)(type)].showSign, bufferForFunc);
        int len = (int)(std::strlen(text)) + (int)(std::strlen(buffer)) + 1;
        if (len + 1 <= lenBuf)
        {
            std::strcat(buffer, text);
        }
    }
    else
    {
        return String(buffer);
    }

    return String(buffer);
}


char* AutoMeasurements::Freq2String(float freq, bool, char buffer[20])
{
    std::strcpy(buffer, Frequency(freq).ToString().c_str());
    return buffer;
}


char* Time2String(float time, bool always, char buffer[20])
{
    std::strcpy(buffer, Time(time).ToString(always).c_str());
    return buffer;
}


char* Voltage2String(float voltage, bool always, char buffer[20])
{
    std::strcpy(buffer, Voltage(voltage).ToString(always).c_str());
    return buffer;
}


char* Phase2String(float phase, bool, char buffer[20])
{
    return Phase(phase).ToString(buffer);
}


char* AutoMeasurements::Float2String(float value, bool always, char buffer[20])
{
    std::strcpy(buffer, Float(value).ToString(always, 4).c_str());
    return buffer;
}


static float Divide(float val1, float val2)
{
    float result = val1 / val2;

    if(result == std::numeric_limits<float>::infinity())
    {
        result = Float::ERROR;
    }
    else if(isnan(result))
    {
        result = Float::ERROR;
    }

    return result;
}


bool AutoMeasurements::DataIsSetting()
{
    return isSet;
}


void AutoMeasurements::SetData()
{
    isSet = (DS != nullptr);

    if (isSet)
    {
        CountedToCurrentSettings();

        BitSet64 points = DisplayOsci::PainterData::BytesOnDisplay();
        firstByte = points.sword0;
        lastByte = points.sword1;
        nBytes = lastByte - firstByte;

        if (TBASE_DS >= TBase::MIN_P2P)             // Если находимся в поточечном режме, то нужно брать последние считанные точки для проведения измерений
        {
            for (int i = (int)(BYTES_IN_CHANNEL_DS - 1); i >= 0; --i)
            {
                if (IN_A[i] != VALUE::NONE)                // Если это значение считано
                {
                    lastByte = i;
                    firstByte = lastByte - nBytes;
                    if (firstByte < 0)
                    {
                        firstByte = 0;
                        lastByte = nBytes;
                    }
                    break;
                }
            }
        }
    }
}


Measure AutoMeasurements::GetActiveMeasure()
{
    int row = posActive / TableMeasures::NumCols();
    int col = posActive - row * TableMeasures::NumCols();

    return Measure(row, col);
}


void AutoMeasurements::CountedToCurrentSettings()
{
    const uint NUM_BYTES = BYTES_IN_CHANNEL_DS;

    if (ENABLED_DS_A)
    {
        Smoother::Run(IN_A, OUT_A, NUM_BYTES, S_DISP_NUM_SMOOTH);
        std::memcpy(IN_A, OUT_A, NUM_BYTES);
        CountedToCurrentSettings(ChanA, NUM_BYTES, IN_A, OUT_A);
        LimitationData(OUT_A, NUM_BYTES);
    }

    if (ENABLED_DS_B)
    {
        Smoother::Run(IN_B, OUT_B, NUM_BYTES, S_DISP_NUM_SMOOTH);
        std::memcpy(IN_B, OUT_B, NUM_BYTES);
        CountedToCurrentSettings(ChanB, NUM_BYTES, IN_B, OUT_B);
        LimitationData(OUT_B, NUM_BYTES);
    }
}


void AutoMeasurements::CountedToCurrentSettings(Ch::E ch, uint numBytes, const uint8 *in, uint8 *out)
{
    CountedToCurrentRShift(ch, numBytes, in, out);
}


void AutoMeasurements::LimitationData(uint8 *inOut, uint numBytes)
{
    uint8 *data = inOut;

    for (uint i = 0; i < numBytes; i++)
    {
        if(*data != VALUE::NONE)
        {
            Math::Limitation<uint8>(data, VALUE::MIN, VALUE::MAX);
        }
        data++;
    }
}


void AutoMeasurements::CountedToCurrentRShift(Ch::E ch, uint numBytes, const uint8 *in, uint8 *out)
{
    int16 shiftDS = RSHIFT_DS(ch);
    Range::E rangeDS = RANGE_DS(ch);

    int16 shiftSET = S_RSHIFT(ch);
    Range::E rangeSET = S_RANGE(ch);

    if((shiftDS == shiftSET) && (rangeDS == rangeSET))
    {
        std::memcpy(out, in, numBytes);
    }
    else
    {
        for(uint i = 0; i < numBytes; i++)
        {
            float voltage = VALUE::ToVoltage(in[i], rangeDS, shiftDS);
            out[i] = VALUE::FromVoltage(voltage, rangeSET, shiftSET);
        }
    }
}


TypeMeasure::E AutoMeasuresSender::sended = TypeMeasure::Count;

void AutoMeasuresSender::DesignateForSending(TypeMeasure::E type)
{
    sended = type;
}
