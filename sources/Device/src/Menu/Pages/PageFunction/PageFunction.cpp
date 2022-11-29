#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageRecorder.h"


DEF_PAGE_3( pFunction,                                                                                                                                                      //--- ������� --- //-V2567
    "�������",
    "����� �������",
    PageFreqMeter::self,
    PageFFT::self,
    PageRecorder::self,
    PageName::Function, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFunction::self = static_cast<const Page *>(&pFunction);
