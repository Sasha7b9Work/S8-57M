// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#ifndef WIN32

    #pragma GCC diagnostic ignored "-Winvalid-source-encoding"
    #pragma GCC diagnostic ignored "-Wpadded"
    #pragma GCC diagnostic ignored "-Wc++98-compat-pedantic"
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    #pragma GCC diagnostic ignored "-Wself-assign"
    #pragma GCC diagnostic ignored "-Wmissing-noreturn"
    #pragma GCC diagnostic ignored "-Wmissing-prototypes"
    #pragma GCC diagnostic ignored "-Wdeprecated-copy"
    #pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wglobal-constructors"
    #pragma GCC diagnostic ignored "-Wexit-time-destructors"
    #pragma GCC diagnostic ignored "-Wundefined-func-template"
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
    #pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma GCC diagnostic ignored "-Wcast-align"
    #pragma GCC diagnostic ignored "-Wcast-qual"

#endif


#ifdef WIN32
#define __attribute(x)
#define __attribute__(x)
#endif


#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>


typedef const char *const  pcharc;
typedef unsigned int       uint;
typedef unsigned short int uint16;
typedef signed short int   int16;
typedef unsigned char      uint8;
typedef unsigned char      uchar;
typedef signed char        int8;
typedef const char        *pchar;

typedef void(*pFuncVV)(void);

#define ERROR_VALUE_UINT8 255

#define TIME_UPDATE_KEYBOARD 2   // Время между опросами клавиатуры


#ifdef WIN32
#pragma warning(disable:4201)
#endif

void EmptyFuncVV(void);
