// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Grid
{
    //  онстанты используютс€ дл€ расчЄта размеров, которые остаютс€ одинаковыми при любых размерах и пропорци€х сетки - например, элементов меню
    const int WIDTH = 560;
    const int HEIGHT = 400;
    const int SIZE_CELL = 40;

    void Draw();

    // Ёти значени€ могут мен€тьс€ в зависимости от выставленного режима
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int ChannelCenterHeight();
    int ChannelHeight();
    int FullBottom();
    int MathBottom();
    int MathHeight();
    int ChannelBottom();
    // ¬озвращает нижнюю y-координату окна, в котором нужно выводить предупреждени€
    int BottomForWarnings();
};
