#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include <cstring>


void DataSettings::Fill()
{
    Lval_ENABLED_A(this) = S_CHANNEL_ENABLED_A ? 1U : 0U;
    Lval_ENABLED_B(this) = S_CHANNEL_ENABLED_B ? 1U : 0U;

    INVERSE_A(this)      = S_INVERSE_A ? 1U : 0U;
    INVERSE_B(this)      = S_INVERSE_B ? 1U : 0U;

    Lval_RANGE_A(this)   = S_RANGE_A;
    Lval_RANGE_B(this)   = S_RANGE_B;

    RSHIFT_A(this)       = S_RSHIFT_A;
    RSHIFT_B(this)       = S_RSHIFT_B;

    Lval_TBASE(this)     = S_TIME_BASE;
    TSHIFT(this)         = S_TIME_SHIFT;

    Lval_COUPLE_A(this)  = S_MODE_COUPLE_A;
    Lval_COUPLE_B(this)  = S_MODE_COUPLE_B;

    TRIGLEV_A(this)      = S_TRIG_LEVEL(ChanA);
    TRIGLEV_B(this)      = S_TRIG_LEVEL(ChanB);

    Lval_PEAKDET(this)   = PeakDetMode().IsEnabled() ? PeakDetMode::Enabled : PeakDetMode::Disabled;

    Lval_DIVIDER_A(this) = S_DIVIDER_A;
    Lval_DIVIDER_B(this) = S_DIVIDER_B;

    TIME_TIME(this)      = HAL_RTC::GetPackedTime();
    ENUM_POINTS(this)    = S_ENUMPOINTS_FPGA;
}


bool DataSettings::IsEquals(const DataSettings &ds) const
{
    return
        RANGE_A(this) == Lval_RANGE_A(&ds) &&
        RANGE_B(this) == Lval_RANGE_B(&ds) &&
        RSHIFT_A(this) == RSHIFT_A(&ds) &&
        RSHIFT_B(this) == RSHIFT_B(&ds) &&
        TBASE(this) == TBASE(&ds) &&
        TSHIFT(this) == TSHIFT(&ds) &&
        TRIGLEV_A(this) == TRIGLEV_A(&ds) &&
        TRIGLEV_B(this) == TRIGLEV_B(&ds) &&
        ENUM_POINTS(this) == ENUM_POINTS(&ds) &&
        PEAKDET(this) == PEAKDET(&ds);
}


bool DataSettings::EqualsCurrentSettings() const
{
    DataSettings ds;
    ds.Fill();
    return (this->IsEquals(ds));
}


int DataSettings::BytesInChannel() const
{
    int result = ENUM_TO_REL_POINTS(ENUM_POINTS(this));

    if (PEAKDET_ENABLED(this))
    {
        result *= 2;
    }

    return result;
}


int DataSettings::PointsInChannel() const
{
    return ENUM_TO_REL_POINTS(ENUM_POINTS(this));
}


int DataSettings::NeedMemoryForData() const
{
    int result = 0;

    if (enableA)
    {
        result += BytesInChannel();
    }

    if (enableB)
    {
        result += BytesInChannel();
    }

    return result;
}



#define CHANGE(field, delta, max)                               \
if(delta > 0) { field = (field < (max)) ? ++field : 0; }        \
else          { field = (field > 0)         ? --field : (max); }


void PackedTime::ChangeHours(int delta)
{
    CHANGE(hours, delta, 23);
}


void PackedTime::ChangeMinutes(int delta)
{
    CHANGE(minutes, delta, 59);
}


void PackedTime::ChangeSeconds(int delta)
{
    CHANGE(seconds, delta, 59);
}


void PackedTime::ChangeDay(int delta)
{
    CHANGE(day, delta, 31);
}


void PackedTime::ChangeMonth(int delta)
{
    CHANGE(month, delta, 12);
}


void PackedTime::ChangeYear(int delta)
{
    CHANGE(year, delta, 50);
}


void PackedTime::AddTime(uint timeMS)
{
    static const uint msInSecond = 1000;
    static const uint msInMinute = msInSecond * 60;
    static const uint msInHour = msInMinute * 60;

    if (timeMS >= msInHour)
    {
        uint h = timeMS / msInHour;

        AddHours(h);

        timeMS -= h * msInHour;
    }

    if (timeMS >= msInMinute)
    {
        uint m = timeMS / msInMinute;

        AddMinutes(m);

        timeMS -= m * msInMinute;
    }

    if (timeMS >= msInSecond)
    {
        uint s = timeMS / msInSecond;

        AddSeconds(s);

        timeMS -= s * msInSecond;
    }

    AddMilliseconds(timeMS);
}


void PackedTime::AddHours(uint h)
{
    while (h != 0)
    {
        h--;

        hours++;

        if (hours == 24)
        {
            hours = 0;
            AddDays(1);
        }
    }
}


void PackedTime::AddMinutes(uint m)
{
    while (m != 0)
    {
        m--;

        minutes++;

        if (minutes == 60)
        {
            minutes = 0;
            AddHours(1);
        }
    }
}


void PackedTime::AddSeconds(uint s)
{
    while (s != 0)
    {
        s--;

        seconds++;

        if (seconds == 60)
        {
            seconds = 0;
            AddMinutes(1);
        }
    }
}


void PackedTime::AddMilliseconds(uint m)
{
    while (m != 0)
    {
        m--;

        ms++;

        if (ms == 1000)
        {
            ms = 0;
            AddSeconds(1);
        }
    }
}


void PackedTime::AddDays(uint d)
{
    while (d != 0)
    {
        d--;

        day++;

        if (day == 31)
        {
            day = 0;
            AddMonths(1);
        }
    }
}


void PackedTime::AddMonths(uint m)
{
    while (m != 0)
    {
        m--;

        month++;

        if (month == 13)
        {
            month = 1;
            year++;
        }
    }
}


String PackedTime::ToString() const
{
    return String("%02d:%02d:%02d:%03d", hours, minutes, seconds, ms);
}


void DataSettings::CopyDataFrom(const DataSettings *source)
{
    int numBytes = Math::Min(BytesInChannel(), source->BytesInChannel());

    if((enableA != 0) && (source->enableA != 0))
    {
        std::memcpy(dataA, source->dataA, static_cast<uint>(numBytes));
    }

    if((enableB != 0) && (source->enableB != 0))
    {
        std::memcpy(dataB, source->dataB, static_cast<uint>(numBytes));
    }
}


void DataSettings::Clear()
{
    if (enableA)
    {
        std::memset(dataA, VALUE::NONE, static_cast<uint>(BytesInChannel()));
    }

    if (enableB)
    {
        std::memset(dataB, VALUE::NONE, static_cast<uint>(BytesInChannel()));
    }
}
