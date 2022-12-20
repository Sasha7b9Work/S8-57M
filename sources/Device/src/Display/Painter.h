// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"
#include "common/Display/Font/Font_d.h"


namespace Painter
{
    void BeginScene(int num_field, Color color);

    int CurrentField();

    void EndScene();
};
