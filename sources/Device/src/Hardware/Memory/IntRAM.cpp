#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/IntRAM.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/DeviceSettings.h"
#include <cstring>


static DataSettings ds;


static const uint SIZE_BUFFER = 106 * 1024;
static uint8 buffer[SIZE_BUFFER];

static uint16 *const memAveA = reinterpret_cast<uint16 *>(buffer);                              // 0            = 0k
static uint16 *const memAveB = reinterpret_cast<uint16 *>(buffer + 4 * FPGA::MAX_NUM_POINTS);   // 0   + 4 * 8k = 32k

static uint8 *const memP2PA = reinterpret_cast<uint8 *>(memAveB) + 4 * FPGA::MAX_NUM_POINTS;    // 32k + 4 * 8k = 64k
static uint8 *const memP2PB = memP2PA + 2 * FPGA::MAX_NUM_POINTS;                               // 64k + 2 * 8k = 80k

static uint8 *const memRandA = memP2PA;
static uint8 *const memRandB = memP2PB;

static uint16 *const ave[2] = { memAveA, memAveB };
static uint8 *const rand[2] = { memRandA, memRandB };


uint16 *IntRAM::Averager32k(Chan::E ch)
{
    return ave[ch];
}


uint8 *IntRAM::DataRand(Chan::E ch)
{
    return rand[ch];
}


DataSettings *IntRAM::PrepareForP2P()
{
    ds.Fill();

    ds.dataA = ds.enableA ? memP2PA : nullptr;
    ds.dataB = ds.enableB ? memP2PB : nullptr;

    std::memset(memP2PA, VALUE::NONE, 2 * FPGA::MAX_NUM_POINTS); //-V512
    std::memset(memP2PB, VALUE::NONE, 2 * FPGA::MAX_NUM_POINTS); //-V512

    return &ds;
}
