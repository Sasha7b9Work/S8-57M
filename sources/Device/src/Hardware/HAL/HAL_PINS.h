#pragma once


struct HPort
{
    enum E
    {
        _A, //-V2573
        _B, //-V2573
        _C, //-V2573
        _D, //-V2573
        _E, //-V2573
        _F, //-V2573
        _G, //-V2573
        _H, //-V2573
        Count
    };
};

struct HPin
{
    static const uint16 _0;
    static const uint16 _1;
    static const uint16 _2;
    static const uint16 _3;
    static const uint16 _4;
    static const uint16 _5;
    static const uint16 _6;
    static const uint16 _7;
    static const uint16 _8;
    static const uint16 _9;
    static const uint16 _10;
    static const uint16 _11;
    static const uint16 _12;
    static const uint16 _13;
    static const uint16 _14;
    static const uint16 _15;
};


#ifdef OLD_VERSION
#define PIN_A2         HPort::_D, HPin::_11
#define PIN_A3         HPort::_D, HPin::_12
#define PIN_A4         HPort::_D, HPin::_13
#define PIN_LF1        HPort::_G, HPin::_2
#define PIN_LF2        HPort::_G, HPin::_3
#define PIN_LF3        HPort::_G, HPin::_4      // A1S
#define PIN_A0S        HPort::_G, HPin::_5
#define PIN_P2P         HPort::_G, HPin::_1
#define PIN_TESTER_ON   HPort::_F, HPin::_13
#define PIN_TESTER_I    HPort::_G, HPin::_0
#define PIN_TESTER_U    HPort::_F, HPin::_15
#define PIN_TESTER_PNP  HPort::_F, HPin::_14
#else
#define PIN_A2         HPort::_B, HPin::_13
#define PIN_A3         HPort::_G, HPin::_8
#define PIN_A4         HPort::_C, HPin::_6
#define PIN_LF1        HPort::_C, HPin::_7
#define PIN_LF2        HPort::_C, HPin::_8
#define PIN_LF3        HPort::_A, HPin::_8      // A1S
#define PIN_A0S        HPort::_A, HPin::_10
#define PIN_P2P         HPort::_F, HPin::_11
#define PIN_TESTER_ON   HPort::_A, HPin::_3
#define PIN_TESTER_I    HPort::_B, HPin::_2
#define PIN_TESTER_U    HPort::_B, HPin::_0
#define PIN_TESTER_PNP  HPort::_A, HPin::_6
#endif

#define PIN_TESTER_STR  HPort::_C, HPin::_9
#define PIN_TESTER_DAC  HPort::_A, HPin::_5

#define PIN_POWER       HPort::_E, HPin::_2

#define PIN_A1          HPort::_D, HPin::_10
#define PIN_LFS         HPort::_G, HPin::_6

#define PIN_ADC1_IN2    HPort::_A, HPin::_2
#define PIN_ADC1_IN9    HPort::_B, HPin::_1

#define PIN_ADC3        HPort::_F, HPin::_10
#define PIN_ADC3_IT     HPort::_B, HPin::_11

#define PIN_AT2516_OUT  HPort::_C, HPin::_3
#define PIN_AT2516_IN   HPort::_C, HPin::_2
#define PIN_AT2516_CLK  HPort::_B, HPin::_10
#define PIN_AT2516_CS   HPort::_B, HPin::_12

#define PIN_AD9286_SCK  HPort::_B, HPin::_10
#define PIN_AD9286_DAT  HPort::_C, HPin::_3
#define PIN_AD9286_CS   HPort::_E, HPin::_11

#define PIN_CS_RAM      HPort::_G, HPin::_10

#define PIN_DAC1        HPort::_A, HPin::_4

#define PIN_HCD_DM      HPort::_B, HPin::_14
#define PIN_HCD_DP      HPort::_B, HPin::_15

#define PIN_PAN_READY   HPort::_A, HPin::_7
#define PIN_PAN_DATA    HPort::_C, HPin::_4
#define PIN_CS          HPort::_G, HPin::_12
#define PIN_WR          HPort::_D, HPin::_5
#define PIN_RD          HPort::_D, HPin::_4

#define PIN_D0          HPort::_D, HPin::_14
#define PIN_D1          HPort::_D, HPin::_15
#define PIN_D2          HPort::_D, HPin::_0
#define PIN_D3          HPort::_D, HPin::_1
#define PIN_D4          HPort::_E, HPin::_7
#define PIN_D5          HPort::_E, HPin::_8
#define PIN_D6          HPort::_E, HPin::_9
#define PIN_D7          HPort::_E, HPin::_10

#define PIN_PCD_VBUS    HPort::_A, HPin::_9
#define PIN_PCD_DP      HPort::_A, HPin::_12
#define PIN_PCD_DM      HPort::_A, HPin::_11

#define PIN_SPI3_SCK    HPort::_C, HPin::_10
#define PIN_SPI3_DAT    HPort::_C, HPin::_12
#define PORT_SPI3_CS1   HPort::_D
#define PIN_SPI3_CS1    PORT_SPI3_CS1, HPin::_3
#define PORT_SPI3_CS2   HPort::_G
#define PIN_SPI3_CS2    PORT_SPI3_CS2, HPin::_13

#define PIN_SPI4_CS     HPort::_E, HPin::_11
#define PIN_SPI4_SCK    HPort::_E, HPin::_12
#define PIN_SPI4_MISO   HPort::_E, HPin::_13
#define PIN_SPI4_MOSI   HPort::_E, HPin::_14

#define PIN_USART3_TX   HPort::_D, HPin::_8
#define PIN_USART3_RX   HPort::_D, HPin::_9


struct StructPIN
{
    HPort::E port;
    uint16   pin;
};
