#include "defines.h"
#include "FDrive/FileManager.h"
#include "FDrive/FDrive.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Osci/DeviceSettings.h"
#include "usbh_diskio.h"
#include <cstdio>


namespace FDrive
{

    USBH_HandleTypeDef handleUSBH;
    void* handle = &handleUSBH;

    FATFS USBDISKFatFs;
    char USBDISKPath[4];     // -V112
    bool isConnected;
    bool needMount;
    bool needSaveScreen = false;     // ���� true - ����� ��������� ����� �� ������

    void USBH_UserProcess(USBH_HandleTypeDef*, uint8 id);

    // ������������� ������� ����� ��� ����� nameFile
    void SetTimeForFile(const char* nameFile);

    // ��������� ���������� ������ �� ������
    void SaveScreenToFlash();

    void CreateFileName(char name[256]);

    void ReadRow(uint8 row, uint8 pixels[320]);
}


void FDrive::USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            needMount = 1;

            /*
            if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
            {
                display.ShowWarning(WrongFileSystem);
            }
            else
            {
                gFlashDriveIsConnected = true;
                FM_Init();
                Menu::ChangeStateFlashDrive();
            }
            */
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            f_mount(NULL, static_cast<TCHAR const*>(""), 0);
            break;

        case HOST_USER_DISCONNECTION:
            isConnected = false;
            Menu::ChangeStateFlashDrive();
            break;

        default:
            // ������ �� ���� ������
            break;
    }
}



void FDrive::Mount()
{
    FileManager::Init();
    Menu::ChangeStateFlashDrive();
    if (f_mount(&USBDISKFatFs, static_cast<TCHAR const*>(USBDISKPath), 0) != FR_OK)
    {
//        LOG_ERROR("�� ���� �������������� ����");
    }
}


bool FDrive::IsConnected()
{
    return isConnected;
}



void FDrive::Init()
{
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)    // -V2001
    {
        USBH_Init(&handleUSBH, USBH_UserProcess, 0);
        USBH_RegisterClass(&handleUSBH, USBH_MSC_CLASS);
        USBH_Start(&handleUSBH);
    }
}


void FDrive::DeInit()
{
    USBH_Stop(&handleUSBH);
    USBH_DeInit(&handleUSBH);
    FATFS_UnLinkDriver(USBDISKPath); //-V2001
}


void FDrive::Update()
{
    if (needMount)      // ���� ���������� ���������� ����������� �������� �����
    {
        needMount = 0;

        if (f_mount(&USBDISKFatFs, static_cast<TCHAR const*>(USBDISKPath), 1) != FR_OK)
        {
            DISPLAY_SHOW_WARNING("�� ���� ��������� ������. ���������, ��� �� ��� FAT32");
        }
        else
        {
            DISPLAY_SHOW_WARNING("���������� ������������ ����������");
            isConnected = true;
            FileManager::Init();
            Menu::ChangeStateFlashDrive();
        }
    }
    else
    {
        USBH_Process(&handleUSBH);
    }

    if(needSaveScreen)
    {
        SaveScreenToFlash();
        needSaveScreen = false;
    }
}



bool FDrive::AppendStringToFile(pCHAR)
{
    return false;
}



void FDrive::GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;
    

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, fullPath, std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;
        while (true) //-V2530
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if(alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if (fno.fname[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*numDirs)++;
                }
                else
                {
                    (*numFiles)++;
                }
            }
        }
        f_closedir(&dir);
    }
}



bool FDrive::GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s) // -V2506
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if ((numDir == numDirs) && ((pFNO->fattrib & AM_DIR) != 0))
            {
                std::strcpy(nameDirOut, static_cast<const char *>(pFNO->fname));
                f_closedir(pDir);
                return true;
            }
            if (((pFNO->fattrib & AM_DIR) != 0) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameDir(char *nameDirOut, StructForReadDir *s) // -V2506
{
    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(pDir, pFNO) != FR_OK)
        {
            *nameDirOut = '\0';
            f_closedir(pDir);
            return false;
        }
        else if (pFNO->fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if (pFNO->fattrib & AM_DIR)
            {
                std::strcpy(nameDirOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
        }
    }
}



void FDrive::CloseCurrentDir(StructForReadDir *s)
{
    f_closedir(&s->dir);
}



bool FDrive::GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s) // -V2506
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                std::strcpy(nameFileOut, static_cast<const char *>(pFNO->fname));
                f_closedir(pDir);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameFile(char *nameFileOut, StructForReadDir *s) // -V2506
{
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(&s->dir, &s->fno) != FR_OK)
        {
            *nameFileOut = '\0';
            f_closedir(&s->dir);
            return false;
        }
        if (s->fno.fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameFileOut = '\0';
                f_closedir(&s->dir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
            {
                std::strcpy(nameFileOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
        }
    }
}



bool FDrive::OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite) // -V2506
{
    if (f_open(&structForWrite->fileObj, fullPathToFile, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        return false;
    }
    std::strcpy(structForWrite->name, const_cast<char *>(fullPathToFile)); //-V2567
    structForWrite->sizeData = 0;
    return true;
}



bool FDrive::WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite) // -V2506
{
    while (sizeData > 0)
    {
        int dataToCopy = sizeData;
        if (sizeData + structForWrite->sizeData > SIZE_FLASH_TEMP_BUFFER)
        {
            dataToCopy = SIZE_FLASH_TEMP_BUFFER - structForWrite->sizeData;
        }
        sizeData -= dataToCopy;
        std::memcpy(structForWrite->tempBuffer + structForWrite->sizeData, data, static_cast<uint>(dataToCopy));
        data += dataToCopy; // -V102
        structForWrite->sizeData += dataToCopy;
        if (structForWrite->sizeData == SIZE_FLASH_TEMP_BUFFER)
        {
            uint wr = 0;
            if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, static_cast<uint>(structForWrite->sizeData), &wr) != FR_OK || structForWrite->sizeData != static_cast<int>(wr))
            {
                return false;
            }
            structForWrite->sizeData = 0;
        }
    }

    return true;
}



bool FDrive::CloseFile(StructForWrite *structForWrite) // -V2506
{
    if (structForWrite->sizeData != 0)
    {
        uint wr = 0;
        if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, static_cast<uint>(structForWrite->sizeData), &wr) != FR_OK || structForWrite->sizeData != static_cast<int>(wr))
        {
            f_close(&structForWrite->fileObj);
            return false;
        }
    }
    f_close(&structForWrite->fileObj);

    SetTimeForFile(structForWrite->name);

    return true;
}


void FDrive::SetTimeForFile(const char *name)
{
    FILINFO info;

    PackedTime time = HAL_RTC::GetPackedTime();

    info.fdate = static_cast<WORD>(((time.year + 2000 - 1980) * 512) | time.month * 32 | time.day);        // -V112
    info.ftime = static_cast<WORD>(time.hours * 2048 | time.minutes * 32 | time.seconds / 2);              // -V112

    f_utime(name, &info);
}


void FDrive::SaveScreen()
{
    needSaveScreen = true;
}


void FDrive::SaveScreenToFlash()
{
    if(!needSaveScreen)
    {
        return;
    }

    needSaveScreen = false;

    if(!FDrive::IsConnected())
    {
        return;
    }

    HAL_BUS::Panel::ProhibitOtherActions();

#pragma pack(1)
    struct BITMAPFILEHEADER
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    // 54
#pragma pack(4)

    StructForWrite structForWrite;

    char fileName[255];

    CreateFileName(fileName);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile(reinterpret_cast<uint8 *>(&bmFH), 14, &structForWrite);

    FDrive::WriteToFile(reinterpret_cast<uint8 *>(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = { 0 };

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for(int i = 0; i < 32; i++)
    {
        uint color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserved = 0;
        (reinterpret_cast<RGBQUAD *>(buffer))[i] = colorStruct;
    }

    for(int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    for(int row = 239; row >= 0; row--)
    {
        ReadRow(static_cast<uint8>(row), pixels);

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    DISPLAY_SHOW_WARNING("���� ��������");


    HAL_BUS::Panel::AllowOtherActions();
}


void FDrive::CreateFileName(char name[256])
{
    for (int i = 0; ; i++)
    {
        std::snprintf(name, 255, "S857_%03d.bmp", i);

        if (!FDrive::ExistFile("\\", name))
        {
            return;
        }
    }
}


void FDrive::ReadRow(uint8, uint8 [320])
{
    // todo_paint
}


bool FDrive::ExistFile(const char *fullPath, const char *fileName)
{
    int numDirs = 0;
    int numFiles = 0;

    GetNumDirsAndFiles(fullPath, &numDirs, &numFiles);

    char nameFile[100];

    StructForReadDir sfrd;

    for (int i = 0; i < numFiles; i++)
    {
        GetNameFile(fullPath, i, nameFile, &sfrd);

        if (std::strcmp(fileName, nameFile) == 0)
        {
            return true;
        }
    }

    return false;
}
