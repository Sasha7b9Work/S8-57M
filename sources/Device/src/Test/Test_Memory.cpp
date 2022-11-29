#include "defines.h"
#include "Osci/DeviceSettings.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>



struct TestMemoryStruct
{
    static void FillData(DataSettings *ds);
    static bool CheckData(const DataSettings *ds);

};


static void PrepareDS(DataSettings *ds);
// ������ ������ � RAM ��� �������� "0"
static DataSettings *CreateDataInRAM(DataSettings *ds);


bool Test::RAM::Test() //-V2506
{
    ::RAM::Init();

    int numRecord = 5000;

    for (int i = 1; i <= numRecord; i++)
    {
        static int line = -1;

        if (i % 50 == 0)
        {
            line = Display::AddMessage(String("������ %d �� %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).c_str(), line); //-V2564
        }

        DataSettings *ds = ::RAM::PrepareForNewData();

        PrepareDS(ds);

        TestMemoryStruct::FillData(ds);

        for (int j = 0; j < ::RAM::NumberDatas(); j++)
        {
            DataSettings *read = ::RAM::Get(static_cast<int>(std::rand() % ::RAM::NumberDatas()));

            if (read && !TestMemoryStruct::CheckData(read))
            {
                return false;
            }
        }
    }

    ::RAM::Init();

    return true;
}


bool Test::ROM::Data::Test() //-V2506
{
    Display::AddMessage("������ ������");

    ::ROM::Data::EraseAll();

    int numRecord = 300;

    for (int i = 1; i <= numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("������ %d �� %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).c_str(), num); //-V2564

        uint numInROM = std::rand() % ::ROM::Data::MAX_NUM_SAVED_WAVES;

        DataSettings ds;

        ::ROM::Data::Save(numInROM, CreateDataInRAM(&ds));

        for (uint j = 0; j < ::ROM::Data::MAX_NUM_SAVED_WAVES; j++)
        {
            const DataSettings *dsRead = ::ROM::Data::Read(j);

            if (dsRead)
            {
                if (!TestMemoryStruct::CheckData(dsRead))
                {
                    return false;
                }
            }
        }
    }
    
    Display::AddMessage("C����� ������");

    ::ROM::Data::EraseAll();

    return true;
}


#define FILL(en, dat)                                       \
    if (en) {                                               \
        uint8 *data = dat;                                  \
        data[0] = static_cast<uint8>(std::rand());          \
        for (int i = 1; i < numPoints; i++)                 \
            { data[i] = static_cast<uint8>(data[0] * i); }  \
    }

void TestMemoryStruct::FillData(DataSettings *ds)
{
    int numPoints = ds->BytesInChannel();

    FILL(ds->enableA, ds->dataA); //-V2563
    FILL(ds->enableB, ds->dataB); //-V2563
}


#define CHECK(en, dat)                                                              \
    if (en) {                                                                       \
        uint8 *data = dat;                                                          \
        for (int i = 1; i < numPoints; i++)                                         \
            { if (data[i] != static_cast<uint8>(data[0] * i)) { return false; } }   \
    }


bool TestMemoryStruct::CheckData(const DataSettings *ds) //-V2506
{
    int numPoints = ds->BytesInChannel();

    CHECK(ds->enableA, ds->dataA); //-V2563
    CHECK(ds->enableB, ds->dataB); //-V2563

    return false;
}


static void PrepareDS(DataSettings *ds)
{
    ds->enableA = static_cast<uint>(std::rand() % 2);
    ds->enableB = ds->enableA ? static_cast<uint>(std::rand() % 2) : 1;
    ds->peackDet = static_cast<uint>(PeakDetMode::Disabled);
    ds->enumPoints = static_cast<uint>(std::rand() % ENumPointsFPGA::Count);
}


static DataSettings *CreateDataInRAM(DataSettings *ds)
{
    PrepareDS(ds);
    ::RAM::PrepareForNewData();
    TestMemoryStruct::FillData(ds);
    return ds;
}
