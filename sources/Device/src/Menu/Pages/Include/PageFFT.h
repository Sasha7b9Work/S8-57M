// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Page;


struct ScaleFFT
{
#define S_FFT_SCALE         (set.fft.scale)
#define S_FFT_SCALE_IS_LOG  (S_FFT_SCALE == ScaleFFT::Log)

    enum E
    {
        Log,           // ��� �������� �������� ��������������� ����� ������ �������.
        Linear         // ��� �������� �������� �������� ����� ������ �������.
    };
};


struct SourceFFT
{
#define S_FFT_SOURCE        (set.fft.source)
#define S_FFT_SOURCE_IS_A   (S_FFT_SOURCE == SourceFFT::A)
#define S_FFT_SOURCE_IS_B   (S_FFT_SOURCE == SourceFFT::B)

    enum E
    {
        A,
        B,
        Both
    };
};


struct WindowFFT
{
#define S_FFT_WINDOW                (set.fft.window)
#define S_FFT_WINDOW_IS_HAMMING     (S_FFT_WINDOW == WindowFFT::Hamming)
#define S_FFT_WINDOW_IS_BLACKMAN    (S_FFT_WINDOW == WindowFFT::Blackman)
#define S_FFT_WINDOW_IS_HANN        (S_FFT_WINDOW == WindowFFT::Hann)

    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    };
};


struct MaxDBFFT
{
#define S_FFT_MAX_DB    (set.fft.maxDB)

    enum E
    {
        _40,
        _60,
        _80
    };

    static float MaxDBforFFT(MaxDBFFT::E maxDB)
    {
        static const float arrayMAX_DB_FOR_FFT[] = { -40.0F, -60.0F, -80.0F };

        return arrayMAX_DB_FOR_FFT[maxDB];
    }
};


#define S_FFT_ENABLED               (set.fft.enabled)
#define S_FFT_ACTIVE_CURSOR         (set.fft.cursor)
#define S_FFT_ACTIVE_CURSOR_IS_0    (S_FFT_ACTIVE_CURSOR == 0)

#define S_FFT_POS_CUR(num)      (set.fft.posCur[num])
#define S_FFT_POS_CUR_0         (S_FFT_POS_CUR(0))
#define S_FFT_POS_CUR_1         (S_FFT_POS_CUR(1))
#define S_FFT_POS_ACTIVE_CURSOR (S_FFT_POS_CUR(S_FFT_ACTIVE_CURSOR))


struct SettingsFFT
{ //-V802
    bool            enabled;
    uint8           posCur[2];      // ������� ������� �������. ���������� 0...256.
    ScaleFFT::E     scale;
    SourceFFT::E    source;
    WindowFFT::E    window;
    MaxDBFFT::E     maxDB;
    uint8           cursor;         // ����������, ����� �������� ������� ��������� ����� ���������.
};


namespace PageFFT
{
    extern const Page *const self;

    namespace Cursors
    {
        extern const Page *const self;
    };
};
