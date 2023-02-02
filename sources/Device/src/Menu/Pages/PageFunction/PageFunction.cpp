// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageRecorder.h"


DEF_EMPTY_ITEM(e4, &PageFunction::self)
DEF_EMPTY_ITEM(e5, &PageFunction::self)


DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFreqMeter::self,
    PageFFT::self,
    PageRecorder::self,
    &e4,
    &e5,
    PageName::Function, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::HandlerKeyEvent
)

const Page * const PageFunction::self = (const Page *)&pFunction;
