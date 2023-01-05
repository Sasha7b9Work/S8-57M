#include "defines.h"
#include "common/Command.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FlashDrive/FlashDrive.h"
#include "common/Display/Font/Font_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <ff.h>
#include <cmath>
#include <cstdlib>



struct TypeWelcomeScreen
{
    enum E
    {
        Vague,
        Wave,
        VagueWave
    };
};


static bool   isRun;
static float  value;
static float  direction;
static uint   timePrev;


static void DrawProgressBar(uint dT);
static void DrawBigMNIPI();
static int RandValue(int min, int max);
//static void InitPoints();


struct Vector
{
    uint16 x;
    uint8  y;
    uint8  notUsed;
};

#ifdef MSVC
#define __attribute__(x)
#endif

static int numPoints = 0;
static Vector array[7000] __attribute__ ((section("CCM_DATA")));

static TypeWelcomeScreen::E typeScreen = TypeWelcomeScreen::Wave;

#define VAGUE (typeScreen == TypeWelcomeScreen::Vague)
#define WAVE (typeScreen == TypeWelcomeScreen::Wave)
#define ALL (typeScreen == TypeWelcomeScreen::VagueWave)
#define VAGUE_OR_ALL (VAGUE || ALL)
#define WAVE_OR_ALL (WAVE || ALL)


void Display::Init()
{
    value = 0.0F;
    isRun = false;
    timePrev = 0;
    direction = 10.0F;

    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).LoadValueRGB();
    }

    SBuffer(Command::Display_Brightness, 110).Send();

    Painter::BeginScene(Color::BACK);

    DFont::Set(DTypeFont::_GOST28);
    DFont::SetSpacing(3);

    int x0 = 85;
    int dX = 65;
    int y0 = 50;

    int x1 = 120;
    int y1 = 130;

    Text("���").Draw(x0, y0, Color::FILL);

    Text("�����").Draw(x0 + dX, y0);

    Text("�8-57").Draw(x1, y1);

    Painter::EndScene();

    DFont::Set(DTypeFont::_8);
    DFont::SetSpacing(1);
}



static void DrawButton(int x, int y, pchar text)
{
    int width = 25;
    int height = 20;

    Rectangle(width, height).Draw(x, y);
    Text(text).DrawInCenterRect(x, y, width + 2, height - 1);
}


void Display::Update()
{
    isRun = true;

    uint dT = TIME_MS - timePrev;
    timePrev = TIME_MS;

    Painter::BeginScene(Color::BLACK);

    Color::WHITE.SetAsCurrent();

    if (FDrive::State() == State::Start || FDrive::State() == State::Ok)
    {
        Painter::BeginScene(Color::BACK);
        Rectangle(319, 239).Draw(0, 0, Color::FILL);
        DrawBigMNIPI();
        Text("��� ��������� ������ ������� � ����������� ������ ������").DrawInCenterRect(0, 180, Display::WIDTH, 20, Color::WHITE);
        Text("����� ����������: ���./����. 8-017-237-23-40").DrawInCenterRect(0, 205, Display::WIDTH, 20);
        Text("������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-237-22-15").DrawInCenterRect(0, 220, Display::WIDTH, 20);
        Painter::EndScene();
    }
    else if (FDrive::State() == State::Mount)
    {
        DrawProgressBar(dT);
    }
    else if (FDrive::State() == State::WrongFlash)
    {
        Text("�� ������� ��������� ����").DrawInCenterRect(0, 0, Display::WIDTH, 200, Color::FLASH_10);
        Text("���������, ��� �������� ������� FAT32").DrawInCenterRect(0, 20, Display::WIDTH, 200, Color::WHITE);
    }
    else if (FDrive::State() == State::RequestAction)
    {
        Text("���������� ����������� �����������").DrawInCenterRect(0, 0, Display::WIDTH, 200);
        Text("���������� ���?").DrawInCenterRect(0, 20, Display::WIDTH, 200);

        DrawButton(290, 55, "��");
        DrawButton(290, 195, "���");
    }
    else if (FDrive::State() == State::Upgrade)
    {
        Text("��������� ����������").DrawInCenterRect(0, 0, Display::WIDTH, 190);
        Text("��������� ������������ �����������").DrawInCenterRect(0, 0, Display::WIDTH, 220);

        int height = 30;
        int fullWidth = 280;
        int width = (int)(fullWidth * FDrive::PercentsUpdated());

        Region(width, height).Fill(20, 130);
        Rectangle(fullWidth, height).Draw(20, 130);
    }

    Painter::EndScene();
    isRun = false;
}



void DrawProgressBar(uint dT)
{
    const int WIDTH = 300;
    const int HEIGHT = 20;
    const int X = 10;
    const int Y = 200;
    
    float step = dT / direction;

    value += step;

    if (direction > 0.0F && value > WIDTH)
    {
        direction = -direction;
        value -= step;
    }
    else if (direction < 0.0F && value < 0)
    {
        direction = -direction;
        value -= step;
    }
    else
    {
        // ������ ���
    }

    int dH = 15;
    int y0 = 50;

    Text("��������� USB-����.").DrawInCenterRect(X, y0, WIDTH, 10, Color::WHITE);
    Text("��� ����� ������������ �����������").DrawInCenterRect(X, y0 + dH, WIDTH, 10);
    Text("���������...").DrawInCenterRect(X, y0 + 2 * dH, WIDTH, 10);

    Rectangle(WIDTH, HEIGHT).Draw(X, Y);
    Region((int)(value), HEIGHT).Fill(X, Y);
}



bool Display::IsRun()
{
    return isRun;
}



static void DrawBigMNIPI()
{
    static uint startTime = 0;
    static bool first = true;
    
    if(first)
    {
        first = false;
        startTime = TIME_MS;
    }

    uint time = TIME_MS - startTime;

    int numColor = (int)(time / (float)TIME_WAIT * 14.0F);
    Limitation(&numColor, 0, 13);



    ((Color)((uint8)(numColor + 2))).SetAsCurrent();

    float amplitude = 3.0F - (time / (TIME_WAIT / 2.0F)) * 3;
    LIMIT_BELOW(amplitude, 0.0F);
    float frequency = 0.05F;

    float radius = 5000.0F * (TIME_WAIT) / 3000.0F / time;
    LIMIT_BELOW(radius, 0);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = WAVE_OR_ALL ? amplitude * std::sinf(frequency * time + i / 5.0F) : 0;
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = array[i].x + (VAGUE_OR_ALL ? RandValue((int)(-radius), (int)(radius)) : 0) + (int)(shift[array[i].y]); //-V537
        int y = array[i].y + (VAGUE_OR_ALL ? RandValue((int)(-radius), (int)(radius)) : 0);
        if (x > 0 && x < 319 && y > 0 && y < 239)
        {
            Pixel().Draw(x, y);
        }
    }
}


static int RandValue(int min, int max)
{
    int val = (int)(std::rand() % (max - min + 1));

    return val + min;
}
