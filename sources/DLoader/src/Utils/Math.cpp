#include "defines.h"
#include "Math.h"
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <limits>



int LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & ((int)(value)))
        {
            return i;
        }

        verValue <<= 1;
    }

    return -1;
}


void Math::Smoothing(uint8 *data, int numPoints, int numSmooth)
{
    if (numSmooth == 0 || numSmooth == 1)
    {
        return;
    }

    float *buffer = static_cast<float *>(std::malloc(static_cast<size_t>(numPoints * sizeof(float)))); //-V2511
    int  *num = static_cast<int *>(std::malloc(static_cast<size_t>(numPoints * sizeof(int)))); //-V2511

    for (int i = 1; i < numPoints; i++)
    {
        buffer[i] = 0.0F; //-V522

        num[i] = 0; //-V522
        for (int j = -numSmooth / 2; j < numSmooth / 2; j++)
        {
            int index = i + j;
            if (index >= 1 && index < numPoints)
            {
                buffer[i] += data[index];
                ++num[i];
            }
        }
    }
    
    for (int i = 1; i < numPoints; i++)
    {
        data[i] = static_cast<uint8>(buffer[i] / num[i] + 0.5F);
    }

    std::free(buffer); //-V2511
    std::free(num); //-V2511
}


bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}


float MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}


int Math::MinFrom2Int(int val0, int val1)
{
    if (val0 < val1)
    {
        return val0;
    }
    return val1;
}


int Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}


uint8 Math::MaxFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MAX_IF_ABOVE if(d > max) { max = d; }

    uint8 max = 0;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MAX_IF_ABOVE;
        d = *pointer++;
        MAX_IF_ABOVE;
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MAX_IF_ABOVE
    }

    return max;
}


uint8 Math::MinFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MIN_IF_LESS if(d < min) { min = d; }

    uint8 min = 255;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MIN_IF_LESS
            d = *pointer++;
        MIN_IF_LESS
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MIN_IF_LESS
    }

    return min;
}


float RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((std::rand() / (float)RAND_MAX) * delta);
}


float Math::GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine)
{
    if (y0 == y1)
    {
        return static_cast<float>(x1);
    }

    return (yHorLine - y0) / (static_cast<float>(y1 - y0) / static_cast<float>(x1 - x0)) + x0;
}


uint8 Math::CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        return data[x];
    }

    int count = 1;
    int sum = data[x];
    int startDelta = 1;

    int endDelta = numSmoothing / 2;

    for (int delta = startDelta; delta <= endDelta; delta++)
    {
        if (((x - delta) >= 0) && ((x + delta) < (numPoints)))
        {
            sum += data[x - delta];
            sum += data[x + delta];
            count += 2;
        }
    }

    if ((numSmoothing % 2) == 1)
    {
        int delta = numSmoothing / 2;
        if ((x + delta) < numPoints)
        {
            sum += data[x + delta];
            count++;
        }
    }

    return static_cast<uint8>(sum / static_cast<float>(count));
}


void Math::CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        std::memcpy(dataOut, dataIn, static_cast<size_t>(numPoints));
    }
    else
    {
        bool addCalculation = (numSmoothing % 2) == 1;
        int endDelta = numSmoothing / 2;
        int startDelta = 1;
        int d = numSmoothing / 2;

        for (int i = 0; i < numPoints; i++)
        {
            int count = 1;
            int sum = dataIn[i];

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (numPoints)))
                {
                    sum += dataIn[i - delta];
                    sum += dataIn[i + delta];
                    count += 2;
                }
            }

            if (addCalculation)
            {
                if ((i + d) < numPoints)
                {
                    sum += dataIn[i + d];
                    count++;
                }
            }

            dataOut[i] = static_cast<uint8>(sum / static_cast<float>(count));
        }
    }
}


uint8 Math::MaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint)
{
    uint8 max = 0;

    const uint16 *pointer = &data[firstPoint];

    const int endPoint = lastPoint / 2;

    for (int i = firstPoint; i < endPoint; i++)
    {
        uint16 d16 = *pointer++;

        uint8 d8 = static_cast<uint8>(d16);
        if (d8 > max)
        {
            max = d8;
        }

        d8 = static_cast<uint8>(d16 >> 8);
        if (d8 > max)
        {
            max = d8;
        }
    }

    return max;
}


uint8 Math::MinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint)
{
    uint8 min = 255;

    const uint16 *pointer = &data[firstPoint];

    const int endPoint = lastPoint / 2;

    for (int i = firstPoint; i < endPoint; i++)
    {
        uint16 d16 = *pointer++;

        uint8 d8 = static_cast<uint8>(d16);
        if (d8 < min)
        {
            min = d8;
        }
        d8 = static_cast<uint8>(d16 >> 8);
        if (d8 < min)
        {
            min = d8;
        }
    }

    return min;
}


int Math::FindAnotherElement(const uint8 *data, uint8 value, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        if (data[i] != value)
        {
            return i;
        }
    }

    return -1;
}


int Math::DigitsInIntPart(float value)
{
    float absValue = fabsf(value);

    int num = 0;

    do
    {
        ++num;
        absValue /= 10.0F;
    } while (absValue >= 1.0F);

    return num;
}


float Math::RoundFloat(float value, int numDigits)
{
    float absValue = fabsf(value);

    int digsInInt = Math::DigitsInIntPart(absValue);

    if (digsInInt < numDigits)  // �������������
    {
        int pow = Pow10(numDigits - digsInInt);
        absValue = ((int)(absValue * pow + 0.5F)) / static_cast<float>(pow);
    }

    return value > 0.0F ? absValue : -absValue;
}


bool IsEquals(float x, float y)
{
#ifdef WIN32
    return fabsf(x - y) < 0.0001F;
#else
    return fabsf(x - y) < std::numeric_limits<float>::epsilon();
#endif
}


bool FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = fabsf(value0) > fabsf(value1) ? fabsf(value0) : fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return fabsf(value0 - value1) < epsilonAbs;
}
