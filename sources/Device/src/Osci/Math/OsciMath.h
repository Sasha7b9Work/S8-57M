#pragma once


struct DataSettings;


namespace AveragerOsci
{
    void Process(Ch::E, const uint8 *newData, int size);
    void SettingChanged();
};



namespace InterpolatorLinear
{
    void Run(DataSettings *ds);
};


namespace InterpolatorSinX_X
{
    void Run(DataSettings &ds);
};
