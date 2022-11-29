#include "defines.h"
#include "log.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Math/OsciMath.h"
#include "Utils/Math.h"
#include "Utils/Queue.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>


static void InterpolateChannel(uint8 *data, int numPoints, uint tBase);


void InterpolatorSinX_X::Run(DataSettings *ds) //-V2506
{
    if (!OSCI_IN_MODE_RANDOMIZER)
    {
        return;
    }

    HAL_BUS_CONFIGURE_TO_FSMC();

    int numPoints = ds->PointsInChannel();

    if (ds->enableA)
    {
        InterpolateChannel(ds->dataA, numPoints, ds->tBase);
    }

    if (ds->enableB)
    {
        InterpolateChannel(ds->dataB, numPoints, ds->tBase);
    }
}


static void InterpolateChannel(uint8 *data, int numPoints, uint tBase)
{
    /*
 ������������������ x � sin(x)   // ���, ��������, �����������
2    1. 20�� : pi/2, -pi/2 ...
8    2. 10�� : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 5��  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 2��  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 1��  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = { 50, 20, 10, 5, 2 };
    int delta = deltas[tBase];

    uint8 *signedData = reinterpret_cast<uint8 *>(std::malloc(static_cast<uint>(numPoints) / 2U)); //-V2511
    int numSignedPoints = 0;

    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0) //-V2563
        {
            signedData[numSignedPoints++] = data[pos]; //-V522 //-V2563
        }
    }

    // ����� �������� ������ �������� �����

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0) //-V2563
        {
            shift = pos;
            break;
        }
    }

    float deltaX = Math::PI_F;
    float stepX0 = Math::PI_F / static_cast<float>(delta);
    float x0 = Math::PI_F - stepX0;
    int num = 0;

    for (int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if ((i % delta) == 0)
        {
            data[i] = signedData[i / delta]; //-V2563
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sinf(Math::PI_F / delta * (part + 1)) : std::sinf(Math::PI_F / delta * (part - (delta - 1) * 2)); //-V2564

            if (tBase > TBase::_5ns)                 // ����� ���������� ����� �������, �� ����� ������������ ���������� ������ �����
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint); //-V2563
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF); //-V2563 //-V2564
            }
            else                                    // �� ���� ��������� ���������� � ��������� ������� ��� ���������� ��������������
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x; //-V2563 //-V2564
                    sinX = -sinX;
                }
                data[i] = static_cast<uint8>(value); //-V2563
            }
        }
    }

    int pos = numPoints - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift]; //-V2563
        pos--;
    }

    std::free(signedData); //-V2511
}
