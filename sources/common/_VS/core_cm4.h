#pragma once

#define __CC_ARM //-V2573

#include "stdint.h"

#undef __ASM
#define __ASM() //-V2573

#define __FPU_USED 0 //-V2573

struct SysTickStruct
{
    unsigned short CTRL;
};

#define SysTick    ((SysTickStruct *)0)

#define NVIC_SetPriority(x, y)

inline void NVIC_SystemReset(void) {};


extern struct structSCB
{
    unsigned VTOR;
} *SCB;


#ifndef __STATIC_INLINE
    #define __STATIC_INLINE inline
#endif

#ifdef DEVICE
    inline uint32_t __RBIT(uint32_t value)
    {
        return value;
    }
#endif


#ifndef __CLZ
	#define __CLZ
#endif

