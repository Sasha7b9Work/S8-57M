#pragma once

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-template"
#pragma GCC diagnostic ignored "-Wglobal-constructors"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Winvalid-source-encoding"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

#define DEBUG

#define LANG_RU true

#define VERSION "54-1.0"

#define MATH_FUNC_IS_SUM false
#define MATH_FUNC_IS_MUL false

#ifdef WIN32
    #define __attribute(x)
    #define __attribute__(x)
    #define WCHAR unsigned short
    #define __STATIC_INLINE inline
    #define __RBIT(x) 1
    #define __CLZ(x)  1
#endif


typedef signed   char        int8;
typedef unsigned char        uint8;
typedef const char          *pchar;
typedef unsigned char        uchar;
typedef signed   short int   int16;
typedef unsigned short int   uint16;
typedef unsigned int         uint;
typedef const    char *const pcharc;
typedef uint16               col_val;

typedef void  (*pFuncVV)();
typedef bool  (*pFuncBV)();

#define _GET_BIT(value, bit)                    (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)                    ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << (bit))))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= ((bitValue) << (numBit)))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define NOT_USED 0

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"

#include <stm32f4xx_hal.h>

#define LOG_WRITE(...)
