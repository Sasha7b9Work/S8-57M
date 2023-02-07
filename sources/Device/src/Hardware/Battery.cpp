// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Display/Primitives.h"
#include "Hardware/Battery.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Averager.h"


namespace Battery
{
    static const float MAX_ADC_REL = (float)((1 << 12) - 1);     // Максимальное значение, которое возможно считать с АЦП
    static const float MAX_ADC_ABS = 3.0F;                                  // Напряжение, соответствующее MAX_ADC_REL
    const float Battery::SHUTDOWN_VOLTAGE = 5.4F;

    static bool isBusy = false;

    static Utils::AroundAverager<float> averager(32);

    // Возвращает true, если зарядное устройство подключено
    static bool ChargerIsConnected();

    // Перевод считанного значения ЦАП источника в вольты
    static float ChargerADC_ToVoltage(uint value);

    // Перевод считанного значения ЦАП батареи в вольты
    static float ADC_ToVoltage(float value);

    // Рассчитать процент отставшегося заряда
    static float CalculatePercents(float volts);

    // Отобразить заряд батареи в графическом виде
    static void DrawUGO(int x, int y, float procents, int width);

    static void DrawFilled(int x, int y, int full, float percents);

    // Значения, соответствующие 100% и 0%, для текущих напряжения аккумуляторов akk и источника заряда pow
    static float Voltage100();
    static float Voltage0();
}


void Battery::Init()
{
    HAL_ADC1::Init();
}


bool Battery::IsBusy()
{
    return isBusy;
}


float Battery::GetVoltage()
{
    isBusy = true;

    uint akk = HAL_ADC1::ValueBattery();

    averager.Push((float)akk);

    isBusy = false;

    return ADC_ToVoltage((float)averager.Value());
}


float Battery::GetVoltageAverage()
{
    return ADC_ToVoltage((float)averager.Value());
}


float Battery::CalculatePercents(float volts)
{
    if (volts >= Voltage100())
    {
        return 100.0F;
    }
    else if (volts <= Voltage0())
    {
        return 0.0F;
    }

    volts -= Voltage0();

    return volts / (Voltage100() - Voltage0()) * 100.0F;
}


void Battery::DrawUGO(int x, int y, float percents, int width)
{
    int widthFull = width;
    int widthSmall = width / 7;

    int dY = 9 * 2;

    Rectangle(widthFull - widthSmall, 8 + dY).Draw(x + widthSmall + 1, y - dY, percents <= 25.0F ? Color::RED : Color::BATTERY);
    Rectangle(widthSmall, 4 + dY).Draw(x + 1, y + 2 - dY);

    DrawFilled(x + widthSmall + 3, y - 3, widthFull - widthSmall - 4, percents);
}


void Battery::DrawFilled(int x, int y, int full, float percents)
{
    int filled = (int)(full * percents / 100.0F + 0.5F);

    Region(filled, 17).Fill(x + full - filled, y - 6, percents <= 25.0F ? Color::RED : Color::BATTERY);
}


void Battery::Draw(int x, int y, int width)
{
    float akk = GetVoltage();

    float percents = CalculatePercents(akk);

    DrawUGO(x, y, percents, width);

    if(S_DBG_SHOW_BATTERY)
    {
        Font::Set(TypeFont::Normal);

        Text(String("%4.2f В", akk)).Draw(x + 10, y, Color::FILL);
        Text(String("%4.1f %%", percents)).Draw(x + 10, y + 9);
    }
}


float Battery::ChargerADC_ToVoltage(uint value)
{
    float result = (value / MAX_ADC_REL) * MAX_ADC_ABS;

    result *= 124.0F / 24.0F;

    return result;
}


float Battery::ADC_ToVoltage(float value)
{
    float result = (value / MAX_ADC_REL) * MAX_ADC_ABS;

    result *= 101.1F / 26.1F;

    return result;
}


float Battery::Voltage100()
{
    return ChargerIsConnected() ? 8.301F : 8.3F;
}


float Battery::Voltage0()
{
    return ChargerIsConnected() ? Battery::SHUTDOWN_VOLTAGE : Battery::SHUTDOWN_VOLTAGE; //-V583
}


bool Battery::ChargerIsConnected()
{
    uint pow = HAL_ADC1::ValueCharger();

    return ChargerADC_ToVoltage(pow) > 8.0F;
}
