// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once



typedef enum
{
    TypePage_Content,       // �� ���� �������� ����������� ������ �� ������ ��������
    TypePage_Description    // �� ���� �������� ��� ������, ������ ��������
} TypePageHelp;


#define MAX_PAGES 3


struct PageHelpContent
{
    TypePageHelp       type;
    uint8              notUsed[3];
    void              *parent;              // ����� ������������ ��������
    // \todo ���������� �� ����� ���������
    //pFuncBV            funcNotUsed;         // ��������� ��� ������������ � ����� Page - ����� ��� ���������.
    pchar  const titleHint[2];        // \brief �������� �������� �� ������� � ���������� �����, � ����� ���������� �� ������� � ���������� 
                                            // (��� ������ TypePage_Description)
    void              *pages[MAX_PAGES];    // ������ �������� ������ ��������� ������� � ������ TypePage_Content
    pString Title() const
    {
        return titleHint[0];
    }
    pString Hint() const
    {
        return titleHint[1];
    }
};

extern const PageHelpContent helpMenu;

static const PageHelpContent helpMenuCommon =
{
    TypePage_Description, {},
    (void *)((PageHelpContent *)&helpMenu),
    {
        "����� �������� ��������� ����",
        "������ �� ������ ���������� ����� ��� ���� ������� - ��������, ������������� ����� 0.5 ��� � �������, ������������� ����� 0.5 ���. "
        "���� ������������ ����� ����������� ��������� ���������. "
        "������� �������� ���� ����������� �������� ���� ������� �������� ������ ����. "
        "�� ���� ��������� ������� �������� ������� ���������������� ������ � ������� ������� ����� �� ������ �� ������ ����������:\n"
        "\"�������\" - �������\n"
        "\"����� 1\" - ����� 1\n"
        "\"����� 2\" - ����� 2\n"
        "\"�����.\" - �����\n"
        "\"����E����\" - ����\n"
        "\"�������\" - �������\n"
        "\"������\" - ������\n"
        "\"���������\" - �����\n"
        "\"������\" - ������\n"
        "\"������\" - ������\n"
        "� ������ �������� ������� �������� ������� ��������� ��������������� ���������"
    },
    {}
};

static const PageHelpContent helpMenuControls =
{
    TypePage_Description, {},
    (void *)((PageHelpContent *)&helpMenu),
    {
        "�������� ������� �����������",
        ""
    },
    {}
};

extern const PageHelpContent helpMain;

static const PageHelpContent helpSCPI =
{
    TypePage_Description, {},
    (void *)((PageHelpContent *)&helpMain),
    {
        "������ � SCPI",
        ""
    },
    {}
};

const PageHelpContent helpMenu =
{
    TypePage_Content, {},
    (void *)((PageHelpContent *)&helpMain),
    {
        "������ � ����",
        "Working with menus"
    },
    {
        (void *)((PageHelpContent *)&helpMenuCommon),
        (void *)((PageHelpContent *)&helpMenuControls)
    }
};

const PageHelpContent helpMain =
{
    TypePage_Content, {},
    0,
    {
        "������",
        "HELP"
    },
    {
        (void *)((PageHelpContent *)&helpMenu),
        (void *)((PageHelpContent *)&helpSCPI)
    }
};
