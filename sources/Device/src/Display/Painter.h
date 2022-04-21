#pragma once
#include "Colors.h"
#include "common/Display/Font/Font_d.h"


namespace Painter
{
    void Init();

    void BeginScene(Color);

    void EndScene();

    void DrawTesterData(uint8 mode, Color, const uint16 *x, const uint8 *y);
};
