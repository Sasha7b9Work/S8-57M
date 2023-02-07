// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


class Buffer;


struct PackedTime
{
    unsigned hours    : 5;
    unsigned minutes  : 6;
    unsigned seconds  : 6;
    unsigned year     : 7;
    unsigned month    : 4;
    unsigned notUsed0 : 4;
    unsigned day      : 5;
    unsigned ms       : 27;
    PackedTime(uint h = 11, uint m = 11, uint s = 11, uint d = 11, uint mo = 11, uint y = 11) :
        hours(h), minutes(m), seconds(s), year(y), month(mo), notUsed0(0), day(d), ms(0) {};
    // ��������� �������� ���� �� +/- 1
    void ChangeHours(int delta);
    void ChangeMinutes(int delta);
    void ChangeSeconds(int delta);
    void ChangeDay(int delta);
    void ChangeMonth(int delta);
    void ChangeYear(int delta);

    // ���������� ��������� PackedTime, ����� � ������� ������� � ������� �� timeMS �����������
    void AddTime(uint timeMS);

    String ToString() const;

private:

    // �������� � ���� hours �����
    void AddHours(uint hours);
    void AddMinutes(uint minutes);
    void AddSeconds(uint seconds);
    void AddMilliseconds(uint ms);
    void AddDays(uint days);
    void AddMonths(uint months);
};

struct DataSettings
{
    uint        id;                 // �������� id ������. 1-� ����� �������� ������, ��������� ������� ����� ��������� ������� � ������ ��������� ������������� �� 1
    uint        timeMS;             // ����� ������ ������
    uint8       *ch_a;              // �� ����� ������ �������� ������ 1-�� ������
    uint8       *ch_b;              // �� ����� ������ �������� ������ 2-�� ������. ��� �������� � Buffer ������ 2-�� ������ ���� ����� ����� 1-�� ������
    int16       rShift[2];
    int16       trigLev[2];
    int         tShift;             // �������� �� �������
    Range::E    range[2];           // ������� �� ���������� ����� �������.
    uint        tBase       : 5;    // ������� �� �������
    uint        en_a        : 1;    // ������� �� ����� A
    uint        en_b        : 1;    // ������� �� ����� B
    uint        coup_a      : 2;    // ����� ������ �� �����
    uint        coup_b     : 2;
    uint        peackDet    : 2;    // ������� �� ������� ��������
    uint        inv_a    : 1;
    uint        inv_b    : 1;
    uint        mult_a : 1;
    uint        mult_b : 1;
    uint        enumPoints  : 3;
    uint        numInROM    : 5;    // ����� ������ � ������ ROM
    uint        notUsed     : 7;
    PackedTime  time;
    
    // ��������� ��������� � ������������ � �������� �����������
    void Fill();
    
    // ���������� ������ ���������� ������� ������ ������
    int BytesInChannel() const;
    
    // ���������� �������� �� �����
    int PointsInChannel() const;
    
    // ���������� ��������� ���������� ������ ��� �������� ������ �������
    int NeedMemoryForData() const;
    
    bool IsEquals(const DataSettings &ds) const;
    
    bool EqualsCurrentSettings() const;
    
    uint8 *Data(Ch::E ch) { return ch == ChA ? ch_a : ch_b; }
    
    // ���������� ������ �� source � ��������� ������������
    void CopyDataFrom(const DataSettings *source);

    // ������ ���������� VALUE::AVE
    void Clear();
};


#define DATA(ds, ch)            (((ch) == ChA) ? (ds)->dataA : (ds)->dataB)

#define Lval_ENABLED_A(ds)      ((ds)->en_a)
#define Lval_ENABLED_B(ds)      ((ds)->en_b)
#define Lval_ENABLED(ds, ch)    (ch.IsA() ? Lval_ENABLED_A(ds) : Lval_ENABLED_B(ds))
#define ENABLED_A(ds)           ((bool)Lval_ENABLED_A(ds))
#define ENABLED_B(ds)           ((bool)Lval_ENABLED_B(ds))
#define ENABLED(ds, ch)         (((ch) == ChA) ? ENABLED_A(ds) : ENABLED_B(ds))

#define ENABLED_DS(ch)          (ENABLED(DS, ch))
#define ENABLED_DS_A            ENABLED_A(DS)
#define ENABLED_DS_B            ENABLED_B(DS)

#define RSHIFT(ds, ch)          ((ds)->rShift[(int)(ch)])
#define RSHIFT_A(ds)            (RSHIFT(ds, ChA))
#define RSHIFT_B(ds)            (RSHIFT(ds, ChB))

#define Lval_RANGE(ds, ch)      ((ds)->range[(int)(ch)])
#define Lval_RANGE_A(ds)        (Lval_RANGE(ds, ChA))
#define Lval_RANGE_B(ds)        (Lval_RANGE(ds, ChB))
#define RANGE(ds, ch)           ((Range::E)Lval_RANGE(ds, ch))
#define RANGE_A(ds)             (RANGE(ds, ChA))
#define RANGE_B(ds)             (RANGE(ds, ChB))

#define Lval_TBASE(ds)          ((ds)->tBase)
#define TBASE(ds)               ((TBase::E)(Lval_TBASE(ds)))


#define TIME_TIME(ds)           ((ds)->time)
#define TIME_DAY(ds)            ((ds)->time.day)
#define TIME_HOURS(ds)          ((ds)->time.hours)
#define TIME_MINUTES(ds)        ((ds)->time.minutes)
#define TIME_SECONDS(ds)        ((ds)->time.seconds)
#define TIME_MONTH(ds)          ((ds)->time.month)
#define TIME_YEAR(ds)           ((ds)->time.year)

#define INVERSE_A(ds)           ((ds)->inv_a)
#define INVERSE_B(ds)           ((ds)->inv_b)
#define INVERSE(ds, ch)         (ch.IsA() ? INVERSE_A(ds) : INVERSE_B(ds))

#define TSHIFT(ds)              ((ds)->tShift)

#define Lval_COUPLE_A(ds)       ((ds)->coup_a)
#define Lval_COUPLE_B(ds)       ((ds)->coup_b)
#define COUPLE_A(ds)            ((ModeCouple)Lval_COUPLE_A(ds))
#define COUPLE_B(ds)            ((ModeCouple)Lval_COUPLE_B(ds))
#define COUPLE(ds, ch)          (ch.IsA() ? COUPLE_A(ds) : COUPLE_B(ds))

#define TRIGLEV(ds, ch)         ((ds)->trigLev[ch])
#define TRIGLEV_A(ds)           (TRIGLEV(ds, ChA))
#define TRIGLEV_B(ds)           (TRIGLEV(ds, ChB))

#define Lval_PEAKDET(ds)        ((ds)->peackDet)
#define PEAKDET(ds)             ((PeakDetMode::E)Lval_PEAKDET(ds))
#define PEAKDET_ENABLED(ds)     (PEAKDET(ds) == PeakDetMode::Enabled)

#define Lval_DIVIDER_A(ds)      ((ds)->mult_a)
#define Lval_DIVIDER_B(ds)      ((ds)->mult_b)

#define ENUM_POINTS(ds)         ((ds)->enumPoints)
#define BYTES_IN_CHANNEL(ds)    ((uint)(ds)->BytesInChannel())
