#pragma once

#define DEBUG

#ifdef WIN32
#define __attribute(x) //-V2573
#define __attribute__(x) //-V2573
#endif


typedef const char  *const  pString;
typedef const char *        pCHAR;
typedef unsigned char       uchar;
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;
typedef unsigned int        uint;
typedef unsigned long long  uint64;


#ifdef WIN32
typedef unsigned char  BYTE; //-V677
typedef unsigned short WORD; //-V677
//#define WCHAR unsigned short
#ifndef GUI
typedef unsigned short WCHAR; //-V677
#endif
typedef unsigned long  DWORD; //-V677
typedef unsigned long  FSIZE_t;
typedef unsigned int   UINT; //-V677
typedef unsigned char  uint8_t;
typedef signed   char  int8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#ifndef __IO
#define __IO //-V2573
#endif
#else
#define nullptr 0 //-V1059
#endif


typedef void (*pFuncVV)();
typedef void (*pFuncVB)(bool);
typedef bool (*pFuncBV)();
typedef void (*pFuncVII)(int, int);

void EmptyFuncVV();
void EmptyFuncVB(bool);


#define UNUSED_PARAMETER(x)(void)x


#ifndef _WIN32
#if __ARMCLIB_VERSION < 6070001
#pragma anon_unions
#endif
#endif


#define _SET_BIT_VALUE(value, numBit, bitValue) ((value) |= ((bitValue) << (numBit))) //-V2573
#define _GET_BIT(value, numBit) (((value) >> (numBit)) & 1) //-V2573
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit)))) //-V2573
#define _SET_BIT(value, bit) ((value) |= (1 << (bit))) //-V2573


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define MAX_UINT 0xffffffffU


// ��� ����������� ������� ������� ����� ������������ ����� ���������, ����� ����� ����� ���� ���������, ��� ����� ������ ����������
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]


#define ENABLE_RU "���"

#define DISABLE_RU "����"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

union BitSet16 //-V2514
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
    explicit BitSet16(uint16 v = 0) : halfWord(v) {};
    explicit BitSet16(uint8 v0, uint8 v1) : byte0(v0), byte1(v1) {};
};

union BitSet32 //-V2514
{
    uint    word;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[sizeof(uint)];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };

    explicit BitSet32(uint16 w0, uint16 w1) : halfWord0(w0), halfWord1(w1) {}

    explicit BitSet32(uint w = 0) : word(w) {}

    explicit BitSet32(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0) : byte0(_byte0), byte1(_byte1), byte2(_byte2), byte3(_byte3)
    {
    }

    void Set(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0)
    {
        byte[0] = _byte0;
        byte[1] = _byte1;
        byte[2] = _byte2;
        byte[3] = _byte3;
    }

    void Set(uint _word)
    {
        word = _word;
    }
};

union BitSet64 //-V2514
{
    uint64 dword;
    uint   word[2];
    uint16 halfWord[4];
    uint8  byte[8];
    struct
    {
        int sword0;
        int sword1;
    };
    struct
    {
        uint word0;
        uint word1;
    };
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
        uint16 halfWord2;
        uint16 halfWord3;
    };
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
        uint8 byte5;
        uint8 byte6;
        uint8 byte7;
    };
};



#define RETURN_TO_MAIN_MENU     0
#define RETURN_TO_LAST_MEM      1
#define RETURN_TO_INT_MEM       2
#define RETURN_TO_DISABLE_MENU  3



// � ���� ��� ������� �� ������ ������ ���������� � ������� ��������� ���������� � � ����������
#define HINT_MODE_ENABLED       (gBF.showHelpHints)
#define SHOW_DEBUG_MENU         (gBF.showDebugMenu)
#define NEED_SAVE_TO_FLASHDRIVE (gBF.needForSaveToFlashDrive)
#define EXIT_FROM_SETNAME_TO    (gBF.exitFromModeSetNameTo)
#define SHOW_STRING_NAVIGATION  (gBF.temporaryShowStrNavi)
#define NUM_ROM_SIGNAL          (gBF.currentNumROMSignal)
#define RUN_FPGA_BEFORE_SB      (gBF.runningFPGAbeforeDrawButtons)
#define EXIT_FROM_ROM_TO_RAM    (gBF.exitFromROMtoRAM)
#define ALWAYS_SHOW_ROM_SIGNAL  (gBF.alwaysShowROMSignal)
#define PANEL_CONTROL_RECEIVE   (gBF.panelControlReceive)
#define FPGA_IN_PROCESS_OF_READ (gBF.FPGAinProcessingOfRead)
// ���� 1, �� ������� ��������� � ������ �����
#define CONSOLE_IN_PAUSE        (gBF.consoleInPause)


struct BitField
{
    uint8 currentNumROMSignal;              // ������� ����� �������, ����������� � ����.
    uint showHelpHints                : 1;  // ���� 1, �� ��� ������� ������ ������ ���������� � ������ ��������� ��������� � � ����������.
    uint showDebugMenu                : 1;
    uint needForSaveToFlashDrive      : 1;  // ���� 1, �� ����� ��������� ����� ��������� �� ������.
    uint exitFromModeSetNameTo        : 2;  // \brief ���� ������������ �� ���� ��������� ����� ��� ���������� : 0 - � �������� ����, 1 - 
                                            // � ���� ���������, 2 - � ���� ����� ��, 3 - � ������� ���� � ����������� ����.
    uint temporaryShowStrNavi         : 1;  // ������� ����, ��� ����� �������� ���������� ������ ��������� ����.
    uint runningFPGAbeforeDrawButtons : 1;  // ����� ����������� ���������� � ���, �������� �� ���� ����� ��������� � ����� ������ � �������.
    uint exitFromROMtoRAM             : 1;  // ���� 1, �� �������� �� �������� ���������� ������ ����� �� ����������, � � ���� ���������.
    uint alwaysShowROMSignal          : 1;  // ���� 1, �� ���������� ������ ��������� � ������ "�����. ��" ������.
    uint panelControlReceive          : 1;  // ���� 1, �� ������ �������� �������.
    uint FPGAinProcessingOfRead       : 1;
    uint consoleInPause               : 1;  // ���� 1, �� ������� ��������� � ������ �����.
};

extern volatile BitField gBF;   // @brief ��������� ������� volatile, ������ ��� ����� ��� ��������� ����������� �������� �� ����� ����������. 
                                // ��������, ��� ������� � ���������� soundIsBeep (����� ��������� ������� � ����� ���������� ��������, ����� ��� 
                                // ���������� ������� ��� ��������� (������� ��������� ���������� �� ����������, ���� �����)).


#define INTERRUPT_BEGIN extern "C" {
#define INTERRUPT_END   }

#define CEXPORT_BEGIN 
#define CEXPORT_END 


#define NAME_FILE_FIRMWARE "S8-57.bin"
#define SIZE_FILE_FIRMWARE (128 * 1024 * 4)


#define BUTTON_SIZE wxSize(70, 30)

struct Chan
{
    enum E
    {
        A,
        B,
        Count
    };

    static pString Name(Chan::E ch);
};
