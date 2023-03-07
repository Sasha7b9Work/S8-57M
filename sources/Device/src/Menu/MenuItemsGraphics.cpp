// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Menu/Menu.h"
#include "Utils/Stack.h"
#include "Utils/Values.h"
#include "Hardware/Timer.h"


void GovernorColor::Draw(bool opened) const
{
    int x = X();

    if (opened)
    {
        x = (x + Width() / 2) - widthOpened / 2;

        if (x < 0)
        {
            x = 0;
        }

        DrawOpened(x, Y());
    }
    else
    {
        DrawClosed();
    }
}


void GovernorColor::DrawOpened(int x, int y) const
{
    int width = widthOpened;
    int height = heightOpened;
    OwnData()->ct->Init();
    Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, Color::BACK);
    Rectangle(width, height).Draw(x, y, ColorFrame());
    Region(width - 2, height / 2 - 2).Fill(x + 1, y + 1, OwnData()->ct->color);
    HLine(width).Draw(x, y + Height() / 2 + 2, ColorFrame());
    Text(Title().c_str()).DrawInCenterRect(x + 1, y - 1, width, Height() / 2 + 2, ColorTitleText());
    DrawValue(x + 1, y + 14);
}


void GovernorColor::DrawClosed() const
{
    OwnData()->ct->Init();
    DrawCommonHiPart();
    Region(Width() + 1, Value::HEIGHT - 3).Fill(X() + 1, Y() + 13, OwnData()->ct->color);
}


void GovernorColor::DrawValue(int x, int y) const
{
    ColorType *ct = OwnData()->ct;

    int8 field = ct->currentField;
    const pcharc texts[4] = { "ßð", "Ñí", "Çë", "Êð" };

    uint color = COLOR(ct->color.value);
    uint red = R_FROM_COLOR(color);
    uint green = G_FROM_COLOR(color);
    uint blue = B_FROM_COLOR(color);
    ct->Init();
    int16 vals[4] =
    {
        (int16)(ct->brightness * 100.0F),
        (int16)blue,
        (int16)green,
        (int16)red
    };

    Region(widthOpened - 2, 12).Fill(x, y, Color::BACK);
    x += 98;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(29, 10).Fill(x - 1, y + 1, colorBack);
        String(texts[i]).Draw(x, y + 2, colorDraw);
        Integer(vals[i]).ToString(false, 1).Draw(x + 14, y + 2);
        x -= 32;
    }
}


void Governor::Draw(bool opened) const
{
    if (!IsActive())
    {
        return;
    }

    if (opened)
    {
        DrawOpened(X(), Y());
    }
    else
    {
        DrawClosed();
    }
}


void Governor::DrawOpened(int x, int y) const
{
    DrawCommonHiPart();
    Rectangle(Width() - 1, HeightOpened() - 1).Draw(x, y + 1, ColorFrame());
    DrawLowPart(x, y + 13);
    OwnData()->afterDraw(x - 10, y - 1);
}


void Governor::DrawClosed() const
{
    DrawLowPart(X(), Y() + Value::HEIGHT);
    DrawCommonHiPart();
    OwnData()->afterDraw(X(), Y());
}


void Governor::DrawValue(int x, int y) const
{
    int signGovernor = (GetValue() < 0) ? -1 : 1;

    int value = GetValue();


    if (signGovernor == -1)
    {
        value = -value;
    }

    Font::Set(TypeFont::Small);
    bool sign = (OwnData()->min < 0);

    Integer(OwnData()->max).ToString(sign, 1).Draw(x + 50, y - 5, Color::WHITE);
    Integer(OwnData()->min).ToString(sign, 1).Draw(x + 50, y + 1);

    Font::Set(TypeFont::Normal);

    DrawValueWithSelectedPosition(x + 3, y, value, NumDigits(), currentDigit, true);

    if (sign)
    {
        int startX = x + 40;

        Char(signGovernor < 0 ? (uint8)'\x9b' : (uint8)'\x9a').Draw(startX - 30, y);
    }
}


void Governor::DrawLowPart(int x, int y) const
{
    Region(Width() - 3, Value::HEIGHT - 3).Fill(x + 1, y, ColorBackgroundChoice());

    if (Menu::OpenedItem() != this)
    {
        Color colorTextDown = Color::BLACK;

        if (IsCurrentItem())
        {
            GetSymbol().Draw(x + Width() - 12, y - 1, Color::BACK);
        }

        x = SymbolUGO(SymbolUGO::GOVERNOR_LEFT).Draw(x + 4, y, colorTextDown);

        int delta = (int)(Step());
        if (delta == 0)
        {
            x = Integer(GetValue()).ToString(false, 1).Draw(x + 1, y);
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y - 2;
            int limWidth = Width();
            int limHeight = Value::HEIGHT - 1;
            Color::BLACK.SetAsCurrent();
            if (delta > 0)
            {
                x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
                Text(Integer(NextValue()).ToString(false, 1)).DrawWithLimitation(drawX, y + 10 - delta, limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
                x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
                Text(Integer(PrevValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - 10 - delta, limX, limY, limWidth, limHeight);
            }
        }

        SymbolUGO(SymbolUGO::GOVERNOR_RIGHT).Draw(x + 1, y, colorTextDown);
    }
    else
    {
        DrawValue(x, y);
    }
}


void Choice::Draw(bool opened) const
{
    Item::Draw(opened);

    if (opened)
    {
        DrawOpened();
    }
    else
    {
        DrawClosed();
    }
}


void Choice::DrawOpened() const
{
    int height = HeightOpened();

    Rectangle(Width() - 1, height - 1).Draw(X(), Y(true) + 1, ColorFrame());
    DrawCommonHiPart();

    Region(Width() - 3, height - MOI_HEIGHT_TITLE + 4).Fill(X() + 1, Y(true) + MOI_HEIGHT_TITLE - 5, Color::BACK);
    int8 index = *OwnData()->cell;
    for (int i = 0; i < NumChoices(); i++)
    {
        int yItem = Y(true) + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Region(Width() - 3, MOSI_HEIGHT - 1).Fill(X() + 1, yItem + 2, ColorBackgroundChoice());
        }
        NameSubItem(i).Draw(X() + 4, yItem + 2, pressed ? Color::BLACK : ColorBackgroundChoice());
    }
}


void Choice::DrawClosed() const
{
    if (IsActive())
    {
        Region(Width() - 4, Value::HEIGHT - 2).Fill(X() + 1, Y() + Value::HEIGHT, ColorBackgroundChoice());

        int deltaY = (int)(Step());
        Color colorText = Color::BLACK;
        colorText.SetAsCurrent();
        if (deltaY == 0)
        {
            NameCurrentSubItem().Draw(X() + 5, Y() + Value::HEIGHT + 3);
        }
        else
        {
            Color::BACK.SetAsCurrent();
            Text(NameCurrentSubItem()).DrawWithLimitation(X() + 4, Y() + Value::HEIGHT - deltaY + 1, X(), Y() + 11, Width(), Value::HEIGHT - 1);

            HLine(Item::Width() + 1).Draw(X() + 1, Y() + (deltaY > 0 ? 24 : 19) - deltaY);

            int x = X() + 4;
            int y = Y() + (deltaY > 0 ? (Value::HEIGHT + 13) : 9) - deltaY;

            Text(deltaY > 0 ? NameNextSubItem() : NamePrevSubItem()).DrawWithLimitation(x, y, X(), Y() + 11, Item::Width(), Value::HEIGHT - 1);
        }
    }

    DrawCommonHiPart();

    OwnData()->funcAfterDraw(X(), Y());
}


void Button::Draw(bool) const
{
    Item::Draw(false);

    Region(Width() - 4, Height() - 1).Fill(X() + 1, Y() + 1, ColorTitleBackground());

    Text(Title().c_str()).DrawInCenterRect(X() + 2, Y() + 2, Width(), Height(), ColorTitleText());
}


void EmptyItem::Draw(bool) const
{
    Item::Draw(false);

    Region(Width() - 4, Height() - 1).Fill(X() + 1, Y() + 1, Color::BACK);
}


void GraphButton::Draw(bool) const
{
    Item::Draw(false);

    int x = X() + 1;
    int y = Y() + 1;

    Region(Width() - 4, Height() - 1).Fill(x, y, ColorTitleBackground());

    ColorTitleText().SetAsCurrent();

    OwnData()->funcDraw(x + 45, y + 6);
}


void Page::Draw(bool opened) const
{
    if (opened)
    {
        if (CurrentItemIsOpened())
        {
            Item *item = GetItem(PosCurrentItem());

            item->Draw(opened);
        }
        else
        {
            DrawTitle(Y() - Menu::Title::HEIGHT - 2);

            DrawItems(X(), Y(true));
        }
    }
    else
    {
        Item::Draw(opened);

        Region(Width() - 4, Height() - 1).Fill(X() + 1, Y() + 1, ColorTitleBackground());

        Text(Title().c_str()).DrawInCenterRect(X(), Y() + 2, Width(), Height(), ColorTitleText(), true);
    }
}


void Page::DrawTitle(int y) const
{
    if (!OwnData()->normalTitle())
    {
        return;
    }

    if (Warnings::IsShown())
    {
        y = Grid::Top();
    }

    VLine vline(Menu::Title::HEIGHT - 1);
    vline.Draw(0, y + 1, ColorFrame());
    vline.Draw(Menu::Title::WIDTH, y + 1);

    HLine hline(Menu::Title::WIDTH - 2);
    hline.Draw(1, y);
    hline.Draw(1, y + Menu::Title::HEIGHT + 1);

    Region(Menu::Title::WIDTH - 2, Menu::Title::HEIGHT - 1).Fill(1, y + 1, Color::BACK);

    bool condDrawRSet = NumSubPages() > 1 &&
        !Menu::CurrentItem()->Is(TypeItem::Governor) &&
        Menu::OpenedItem()->Is(TypeItem::Page);

    int delta = condDrawRSet ? -10 : 0;

    Text(Title().c_str()).DrawInCenterRect(0, y + 3, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, ColorTitleText());

    Color::GRAY_75.SetAsCurrent();

    y += 4;

    DrawPagesUGO(Menu::Title::WIDTH - 3, y);
    DrawNestingPage(5, y);
}


void Page::DrawItems(int x, int) const
{
    if (NumItems() == 0)
    {
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        Item *item = GetItem(PosItemOnLeft() + i);

        if (item)
        {
            item->Draw(false);
        }

        x += Width(i);
    }
}


void Item::DrawCommonHiPart() const
{
    bool opened = IsOpened();

    int x = X();
    int y = Y(opened);

    int width = Width() - 4;

    Color colorFill = ColorTitleBackground();
    Color colorText = ColorTitleText();

    Region(width, Item::Value::HEIGHT - 2).Fill(x + 1, y + 1, colorFill);

    Title().Draw(x + 6, y + 4, colorText);

    if (opened)
    {
        HLine(width).Draw(x + 1, y + Item::Value::HEIGHT - 1, ColorFrame());
    }
}


void Governor::DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPosFromEnd, bool) const
{
    Stack<uint8> stack(numDigits);

    for (int i = 0; i < numDigits; i++)
    {
        stack.Push((uint8)(value % 10));
        value /= 10;
    }

    int height = 8;

    for (int i = 0; i < numDigits; i++)
    {
        if (selPosFromEnd == ((int)(numDigits)-(int)(i)-1))
        {
            Region(5, height).Fill(x - 1, y, Color::FILL);
        }

        uint8 val = stack.Pop();

        Char((uint8)(val + 48)).Draw(x, y, Color::BACK);

        x += 6;
    }
}


void Page::DrawPagesUGO(int right, int top) const
{
    int size = 10;
    int delta = 2;

    int allPages = (NumItems() - 1) / Item::NUM_ON_DISPLAY + 1;
    int currentPage = CurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);

    for (int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if (p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}


void Page::DrawNestingPage(int left, int top) const
{
    Page *parent = (Page *)Keeper();

    if (parent != nullptr)
    {
        int nesting = 0;

        while (parent != nullptr)
        {
            const Page *page = parent;
            parent = (Page *)page->Keeper();
            nesting++;                                  // -V127
        }

        int size = 10;
        int delta = 2;

        for (int i = 0; i <= nesting; i++)
        {
            int x = left + i * (size + delta);
            Rectangle(size, size).Draw(x, top);
        }
    }
}


int GraphButton::NumHints() const
{
    if (OwnData()->hintUGO == nullptr)
    {
        return 0;
    }

    const StructHelpDrawButton *hint = OwnData()->hintUGO;

    int result = 0;

    while (hint++)
    {
        result++;
    }

    return result;
}


void GraphButton::DrawHints(int x, int y, int width) const
{
    if (NumHints() == 0)
    {
        return;
    }
    Region(width, 239 - y).Fill(x, y, ColorTitleBackground());
    Rectangle(width, 239 - y).Draw(x, y, ColorFrame());
    const StructHelpDrawButton *structHelp = &OwnData()->hintUGO[0];
    x += 3;
    y += 3;
    for (int i = 0; i < NumHints(); i++)
    {
        Rectangle(GraphButton::Width(), GraphButton::Height()).Draw(x, y);
        structHelp->funcDrawUGO(x, y);

        int yNew = Text(structHelp->helpUGO).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);

        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}


void Item::Draw(bool) const
{
    VLine vline(Height() - 1);
    vline.Draw(X(), Y() + 1, ColorFrame());
    vline.Draw(X() + Width() - 2, Y() + 1);

    HLine hline(Width() - 4);
    hline.Draw(X() + 1, Y());
    hline.Draw(X() + 1, Y() + Height() + 1);
}
