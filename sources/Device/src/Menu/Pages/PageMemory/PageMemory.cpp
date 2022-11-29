#include "defines.h"
#include "Tables.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Beeper.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


int ENumPointsFPGA::BytesInChannel()
{
    int result = PointsInChannel();
    if (S_PEAK_DET_ENABLED)
    {
        result *= 2;
    }
    return result;
}


int ENumPointsFPGA::PointsInChannel()
{
    return ENUM_TO_REL_POINTS(S_ENUMPOINTS_FPGA);
}


static void DrawSetMask();  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawFileMask(int x, int y);
static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������
       void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);



static bool IsActive_Points()
{
    return !PeakDetMode().IsEnabled();
}

void PageMemory::OnChanged_LengthMemoryChannel(bool active) //-V2506
{
    // ���� ������� ������� ��������, �� �� ����� ����������� ������
    if (PeakDetMode().IsEnabled() && !active)
    {
        Display::ShowWarning("����� �������� ����� ������, ��������� ������� ��������");
        return;
    }

    Osci::OnChangedLengthMemoryChannel();
}

static pString namesLengthMemory[] = {"512", "1k", "2k", "4k", "8k", "16k", "32k"};

DEF_CHOICE_5( cPoints,                                                                                                                                               //--- ������ - ����� ---
    "����� ������",
    "����� ���������� �������� ��� ����������� ��������. ��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������.",
    namesLengthMemory[0],
    namesLengthMemory[1],
    namesLengthMemory[2],
    namesLengthMemory[3],
    namesLengthMemory[4],
    S_ENUMPOINTS_FPGA, &PageMemory::self, IsActive_Points, PageMemory::OnChanged_LengthMemoryChannel, Choice::AfterDraw
)


static void Draw_Manager_Tab(int x, int y)
{
    Char(SymbolUGO2::TAB).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bManager_Tab,                                                                                                                   //--- ������ - ����� �� - ������� - Tab ---
    "Tab",
    "������� ����� ���������� � �������",
    &PageDrive::Manager::self, Item::Active, FileManager::Press_Tab, Draw_Manager_Tab
)


static void Draw_Manager_LevelUp(int x, int y)
{
    Char(SymbolUGO2::LEAVE).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bManager_LevelUp,                                                                                                 //--- ������ - ����� �� - ������� - ����� �� �������� ---
    "����� �� ��������",
    "������� � ������������ �������",
    &PageDrive::Manager::self, Item::Active, FileManager::Press_LevelUp, Draw_Manager_LevelUp
)


static void Draw_Manager_LevelDown(int x, int y)
{
    Char(SymbolUGO2::ENTER).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bManager_LevelDown,                                                                                                 //--- ������ - ����� �� - ������� - ����� � ������� ---
    "����� � �������",
    "������� � ��������� �������",
    &PageDrive::Manager::self, Item::Active, FileManager::Press_LevelDown, Draw_Manager_LevelDown
)


static void OnPress_Exit()
{
    Menu::CloseOpenedItem();
    Display::SetDrawMode(DrawMode::Auto);
}

DEF_BUTTON(bExit,
    "�����",
    "",
    &PageDrive::Manager::self, Item::Active, OnPress_Exit
)


static bool IsActive_Drive_Manager()
{
    return FDrive::IsConnected();
}

void PageMemory::OnOpenClose_Drive_Manager(bool)
{
    if (FDrive::IsConnected())
    {
        FDrive::Mount();
        Display::SetDrawMode(DrawMode::Hand, FileManager::Draw);
        ModeRedrawFM::Set(ModeRedrawFM::Full);
    }
    else
    {
        Display::ShowWarning("������� ���������� ����-����");
    }
}

DEF_PAGE_5(pManager,                                                                                                                                   //--- ������ - ����� �� - ������� --- //-V2567
    "�������",
    "��������� ������ � �������� ������� ������������� ����������",
    &bManager_Tab,
    &bManager_LevelUp,
    &bManager_LevelDown,
    &Item::empty,
    &bExit,
    PageName::Memory_Drive_Manager,
    &PageDrive::self, IsActive_Drive_Manager, Page::NormalTitle, PageMemory::OnOpenClose_Drive_Manager, Page::BeforeDraw, FileManager::HandlerKey
)

const Page * const PageDrive::Manager::self = static_cast<const Page *>(&pManager);


static void OnPress_Mask_Delete()
{
    S_MEM_FILE_NAME_MASK[0] = '\0';
}

static void Draw_Delete(int x, int y)
{
    Char(SymbolUGO2::DELETE).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bMask_Delete,                                                                                                                 //--- ������ - ����� �� - ����� - ������� ---
    "�������",
    "������� ��� �������� �������",
    &PageDrive::Mask::self, Item::Active, OnPress_Mask_Delete, Draw_Delete
)


static void OnPress_Mask_Backspace()
{
    int size = static_cast<int>(std::strlen(S_MEM_FILE_NAME_MASK)); //-V2513
    if (size > 0)
    {
        if (size > 1 && S_MEM_FILE_NAME_MASK[size - 2] == 0x07)
        {
            S_MEM_FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            S_MEM_FILE_NAME_MASK[size - 1] = '\0';
        }
    }
}

static void Draw_Backspace(int x, int y)
{
    Char(SymbolUGO2::ARROW_LEFT).Draw(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bMask_Backspace,                                                                                                      //--- ������ - ����� �� - ����� - Backspace ---
    "Backspace",
    "������� ��������� �������� ������",
    &PageDrive::Mask::self, Item::Active, OnPress_Mask_Backspace, Draw_Backspace
)


static void OnPress_Mask_Insert() //-V2506
{
    int index = S_MEM_INDEX_CUR_SYMBOL_MASK;
    uint size = std::strlen(S_MEM_FILE_NAME_MASK); //-V2513
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        S_MEM_FILE_NAME_MASK[size] = Tables::Get(index)[0]; //-V2563
        S_MEM_FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // ��� %nN - ��������� ������
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (S_MEM_FILE_NAME_MASK[size - 1] >= 0x30 && S_MEM_FILE_NAME_MASK[size - 1] <= 0x39) // ���� ����� ������� �����
                {
                    S_MEM_FILE_NAME_MASK[size] = S_MEM_FILE_NAME_MASK[size - 1] - 0x30;
                    S_MEM_FILE_NAME_MASK[size - 1] = 0x07;
                    S_MEM_FILE_NAME_MASK[size + 1] = '\0';
                }
            }
        }
        else
        {
            S_MEM_FILE_NAME_MASK[size] = static_cast<char>(index);
            S_MEM_FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

static void Draw_Insert(int x, int y)
{
    Char(SymbolUGO2::INSERT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bMask_Insert,                                                                                                                //--- ������ - ����� �� - ����� - �������� ---
    "��������",
    "��������� ��������� ������",
    &PageDrive::Mask::self, Item::Active, OnPress_Mask_Insert, Draw_Insert
)


static bool IsActive_Mask()
{
    return S_MEM_FILE_NAMING_MODE_IS_MASK;
}

static void OnOpenClose_Mask(bool open)
{
    if(open)
    {
        Display::SetAddDrawFunction(DrawSetMask);
    }
    else
    {
        Display::RemoveAddDrawFunction();
    }
}

static void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (Tables::Get(index)[0] != ' ') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (Tables::Get(index)[0] != 'a') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (Tables::Get(index)[0] != '%') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // ������ ������ �����������
    position = 0;
    while (index < (Tables::Size() / 4))
    {
        Tables::DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static pString strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    Color::FILL.SetAsCurrent();
    for (int i = 0; i < static_cast<int>(sizeof(strings)) / 4; i++)
    {
        String(strings[i]).Draw(x0 + deltaX, y0 + 100 + deltaY * i);
    }
}

static void DrawFileMask(int x, int y)
{
    char *ch = S_MEM_FILE_NAME_MASK;

    Color::FILL.SetAsCurrent();
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            Char(*ch).Draw(x, y);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Char('%').Draw(x, y);
                x = Char(0x30 | *(ch + 1)).Draw(x, y); //-V2563
                x = Char('N').Draw(x, y);
                ch++;
            }
            else
            {
                x = String(Tables::Get(*ch + 0x40)).Draw(x, y);
            }
        }
        ch++;
    }
    Region(5, 8).Fill(x, y, Color::FLASH_10);
}

static bool HandlerKey_Mask(const KeyEvent &event) //-V2506
{
    if(event.IsArrowUp() || event.IsArrowRight())
    {
        int angle = (event.IsArrowUp() || event.IsArrowRight()) ? 1 : -1;

        OnMemExtSetMaskNameRegSet(angle, Tables::Size() / 4);

        return true;
    }

    return false;
}


/*
DEF_PAGE_SB( pMask,                                                                                                                                       //--- ������ - ����� �� - ����� ---
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    &bDrive_Mask_Exit,      // ������ - ����� �� - ����� - �����
    &bMask_Delete,    // ������ - ����� �� - ����� - �������
    0,
    0,
    &bMask_Backspace, // ������ - ����� �� - ����� - Backspace
    &bMask_Insert,    // ������ - ����� �� - ����� - ��������
    Page::Name::SB_Memory_Drive_Mask, &pDrive, IsActive_Mask, OnPress_Drive_Mask, FuncDrawPage, OnRegSet_Drive_Mask
)
*/


DEF_PAGE_3( pMask,                                                                                                                                        //--- ������ - ����� �� - ����� --- //-V2567
    "�����",
    "����� ����� ����� ��� ��������������� ���������� ������",
    &bMask_Delete,
    &bMask_Backspace,
    &bMask_Insert,
    PageName::Memory_Drive_Mask,
    &PageDrive::self, IsActive_Mask, Page::NormalTitle, OnOpenClose_Mask, Page::BeforeDraw, HandlerKey_Mask
)

const Page * const PageDrive::Mask::self = static_cast<const Page *>(&pMask);


DEF_PAGE_4( pMemory,                                                                                                                                                         //--- ������ --- //-V2567
    "������",
    "������ � ������� � ���������� �������.",
    &cPoints,
    PageRAM::self,
    PageROM::self,
    PageDrive::self,  PageName::Memory, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMemory::self = static_cast<const Page *>(&pMemory);


void PageMemory::SaveSignalToFlashDrive()
{
    if (FDrive::IsConnected())
    {
        if (S_MEM_FILE_NAMING_MODE_IS_MANUALLY)
        {
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SETNAME_TO = 0;
    }
}

static void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (Tables::Get(index)[0] != ' ') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (Tables::Get(index)[0] != 'a') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (Tables::Get(index)[0] != '%') //-V2563
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = String(S_MEM_FILE_NAME).Draw(x0 + deltaX, y0 + 65, Color::FILL);

    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}


static void OnPress_SetName_Exit()
{
    if (EXIT_FROM_SETNAME_TO == RETURN_TO_DISABLE_MENU)
    {
        //Menu::PagePointerFromName(Page::Name::SB_Memory_SetName)->ShortPressOnItem(0);
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_LAST_MEM)
    {
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_INT_MEM)
    {
    }
    else
    {
        //
    }

    EXIT_FROM_SETNAME_TO = RETURN_TO_DISABLE_MENU;
}


static void OnPress_SetName_Delete()
{
    S_MEM_FILE_NAME[0] = '\0';
}

DEF_GRAPH_BUTTON( bSetName_Delete,                                                                                                                              //--- ��� ����� - ������� ---
    "�������",
    "������� ��� �������� �������",
    &PageDrive::SetName::self, Item::Active, OnPress_SetName_Delete, Draw_Delete
)


static void OnPress_SetName_Backspace()
{
    uint size = std::strlen(S_MEM_FILE_NAME); //-V2513
    if (size != 0)
    {
        S_MEM_FILE_NAME[size - 1] = '\0';
    }
}

DEF_GRAPH_BUTTON( bSetName_Backspace,                                                                                                                         //--- ��� ����� - Backspace ---
    "Backspace",
    "������� ��������� ������",
    &PageDrive::SetName::self, Item::Active, OnPress_SetName_Backspace, Draw_Backspace
)


static void OnPress_SetName_Insert()
{
    uint size = std::strlen(S_MEM_FILE_NAME); //-V2513
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        S_MEM_FILE_NAME[size] = Tables::Get(S_MEM_INDEX_CUR_SYMBOL_MASK)[0]; //-V2563
        S_MEM_FILE_NAME[size + 1] = '\0';
    }
}

DEF_GRAPH_BUTTON( bSetName_Insert,                                                                                                                             //--- ��� ����� - �������� ---
    "��������",
    "������ ��������� ������",
    &PageDrive::SetName::self, Item::Active, OnPress_SetName_Insert, Draw_Insert
)


static void OnPress_SetName_Save()
{
    if (FDrive::IsConnected())
    {
        OnPress_SetName_Exit();
        NEED_SAVE_TO_FLASHDRIVE = 1;
    }
}

static void Draw_SetName_Save(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Char(SymbolUGO2::FLASH_DRIVE_BIG).Draw4SymbolsInRect(x + 2, y + 1);
    }
}

DEF_GRAPH_BUTTON( bSetName_Save,                                                                                                                              //--- ��� ����� - ��������� ---
    "���������",
    "���������� �� ���� ��� �������� ������",
    &PageDrive::SetName::self, Item::Active, OnPress_SetName_Save, Draw_SetName_Save
)


static bool HandlerKey_SetName(const KeyEvent &event)
{
    OnMemExtSetMaskNameRegSet(event.Delta(), Tables::Size() / 4 - 7);

    return true;
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    typedef void (*pFuncVpI8I8I8)(int8 *, int8, int8);

    static const pFuncVpI8I8I8 func[3] =
    {
        Math::CircleDecrease<int8>,
        Math::CircleDecrease<int8>,
        Math::CircleIncrease<int8>
    };

    Color::ChangeFlash(true);

    if (S_MEM_INDEX_CUR_SYMBOL_MASK > maxIndex)
    {
        S_MEM_INDEX_CUR_SYMBOL_MASK = static_cast<int8>(maxIndex - 1);
    }
    func[Math::Sign(angle) + 1](&S_MEM_INDEX_CUR_SYMBOL_MASK, 0, static_cast<int8>(maxIndex - 1));
    Beeper::RegulatorSwitchRotate();
}

DEF_PAGE_4( pSetName,                                                                                                             //--- �������� ���������� ��� ����� ����� ����� --- //-V2567
    "",
    "",
    &bSetName_Delete,
    &bSetName_Backspace,
    &bSetName_Insert,
    &bSetName_Save,
    PageName::Memory_SetName, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, HandlerKey_SetName
)

const Page * const PageDrive::SetName::self = static_cast<const Page *>(&pSetName);
