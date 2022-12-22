#include "HAL.h"



void HAL::ErrorHandler(pchar , int)
{
    while (true) {}
}


void HAL::Init()
{
    HAL_BUS::Init();
}
