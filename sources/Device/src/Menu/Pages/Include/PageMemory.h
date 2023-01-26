// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


// ����������� ������������ � ���������� �����
#define ENUM_TO_REL_POINTS(num)    ((int)(1 << ((int)(num) + 10)))


// ����� ������.
struct ModeWork
{
#define S_MEM_MODE_WORK         (set.mem._modeWork)
#define S_MEM_MODE_WORK_IS_DIR  (S_MEM_MODE_WORK == ModeWork::Dir)
#define S_MEM_MODE_WORK_IS_RAM  (S_MEM_MODE_WORK == ModeWork::RAM)
#define S_MEM_MODE_WORK_IS_ROM  (S_MEM_MODE_WORK == ModeWork::ROM)

    enum E
    {
        Dir,        // �������� �����.
        RAM,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
        ROM,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
        Count       // ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
    };
};


// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
#define S_MEM_MODE_BTN_MEMORY           (set.mem._modeBtnMemory)
#define S_MEM_MODE_BTN_MEMORY_IS_SAVE   (S_MEM_MODE_BTN_MEMORY == ModeBtnMemory::Save)

    enum E
    {
        Menu,     // ����� ������������ ��������������� �������� ����.
        Save      // ���������� ����������� ������ �� ������.
    };
};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
#define S_MEM_MODE_SAVE_SIGNAL          (set.mem._modeSaveSignal)
#define S_MEM_MODE_SAVE_SIGNAL_IS_BMP   (S_MEM_MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)

    enum E
    {
        BMP,
        TXT
    };
};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // ���������� ������ ��������� �������.
        Saved,   // ���������� ���������� ������.
        Both     // ���������� ��� �������.
    };
};


// ����� ������������ ������.
struct FileNamingMode
{
#define S_MEM_FILE_NAMING_MODE              (set.mem._fileNamingMode)
#define S_MEM_FILE_NAMING_MODE_IS_MASK      (S_MEM_FILE_NAMING_MODE == FileNamingMode::Mask)
#define S_MEM_FILE_NAMING_MODE_IS_MANUALLY  (S_MEM_FILE_NAMING_MODE == FileNamingMode::Manually)

    enum E
    {
        Mask,        // ��������� �� �����.
        Manually     // ��������� �������.
    };
};


// ����� ����� �������, � ������� ��� ������.
struct ENumPointsFPGA
{
#define S_ENUMPOINTS_FPGA   (set.mem._enumPoints)

    enum E
    {
        _1k,
        _2k,
        _4k,
        _8k,
        Count
    };
    static int BytesInChannel();
    static int PointsInChannel();
};


// ����� ������ ���������� � ������ ������ - ����� ��
struct TypeSignalROM
{
#define S_MEM_TYPE_SIGNAL_ROM               (set.mem._typeSignalROM)
#define S_MEM_TYPE_SIGNAL_ROM_IS_CURRENT    (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Current)
#define S_MEM_TYPE_SIGNAL_ROM_IS_RECORDED   (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Recorded)
#define S_MEM_TYPE_SIGNAL_ROM_IS_BOTH       (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Both)

    enum E
    {
        Recorded,
        Current,
        Both,
        Count
    };
};

#define MAX_SYMBOLS_IN_FILE_NAME 35

#define S_MEM_FILE_NAME             (set.mem._fileName)
#define S_MEM_FILE_NAME_MASK        (set.mem._fileNameMask)
#define S_MEM_FLASH_AUTO_CONNECT    (set.mem._flashAutoConnect)
#define S_MEM_INDEX_CUR_SYMBOL_MASK (set.mem._indexCurSymbolNameMask)

struct SettingsMemory
{
    ENumPointsFPGA::E   _enumPoints;                             // ����� �����
    ModeBtnMemory::E    _modeBtnMemory;
    TypeSignalROM::E    _typeSignalROM;                          // ����� ������ �������� � ������ ������-����� ��
    ModeWork::E         _modeWork;                               // ����� ������.
    bool                _flashAutoConnect;                       // ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                _indexCurSymbolNameMask;                 // ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E   _modeSaveSignal;                         // � ����� ���� ��������� ������.
    char                _fileName[MAX_SYMBOLS_IN_FILE_NAME];     // ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E   _;                                      // ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
    FileNamingMode::E   _fileNamingMode;                         // ����� ���������� ������.
    char                _fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // \brief ����� ����� ��� ��������������� ���������� ������.
                        // \details ������� ����������.\n
                        // \code
                        // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������
                        // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007
                        // �������
                        // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32
                        // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� 
                        // ����� ������ ����� 0001, �.�. ��� ����������� ������ ���������������.
                        // \endcode
};


namespace PageMemory
{
    void OnChanged_LengthMemoryChannel(bool active);

    void OnOpenClose_Drive_Manager(bool enter = true);

    void SaveSignalToFlashDrive();

    extern const Page * const self;
};



namespace PageRAM
{
    extern const Page * const self;
};



namespace PageROM
{
    extern const Page * const self;
};



namespace PageDrive
{
    extern const Page * const self;

    namespace Manager
    {
        extern const Page * const self;
    };

    namespace Mask
    {
        extern const Page * const self;
    };

    namespace SetName
    {
        extern const Page *const self;
    };
};
