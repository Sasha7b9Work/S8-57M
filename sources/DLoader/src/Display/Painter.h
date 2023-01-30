#pragma once
#include "Colors.h"
#include "common/Display/Font/Font.h"


struct Painter
{
    static void BeginScene(Color);

    static void EndScene();
};
