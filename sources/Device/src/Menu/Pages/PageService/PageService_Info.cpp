// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/common_defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cstdio>


static void Information_Draw()
{
    for (int i = 0; i < 5; i++)
    {
        Painter::BeginScene(i, Color::BACK);
        int x = 140;
        int dY = 30;
        int y = 70;
        Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
        y += dY;
        y += dY;

        char buffer[100];

        std::sprintf(buffer, MODEL_RU);
        String(buffer).Draw(x, y);

        y += 2 * dY;

        String("Программное обеспечение : версия %s", VERSION).Draw(x, y);

        y += dY - 1;

        String("CRC32 %s", CRC32_DEVICE).Draw(x + 123, y);

        Menu::Draw(4);
        Painter::EndScene();
    }
}


static void OnPress_Close()
{
    Menu::CloseOpenedItem();
}

DEF_BUTTON( bClose,
    "Закрыть",
    "Закрыть страницу информации",
    &PageService::Information::self, Item::Active, OnPress_Close
)

static void OnOpenClose_Page(bool enter)
{
    if (enter)
    {
        Display::AddFunctionRender::Set(Information_Draw);
    }
    else
    {
        Display::AddFunctionRender::Remove();
    }
}

DEF_PAGE_1 ( pInfo,
    "ИНФОРМАЦИЯ",
    "Информация об изделии",
    &bClose,
    PageName::Service_Info, &PageService::self, Item::Active, Page::NormalTitle, OnOpenClose_Page, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageService::Information::self = (const Page *)&pInfo;
