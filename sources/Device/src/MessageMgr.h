// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct MessageMgr
{
    // Вызывается при изменении настройки осциллографа, существенно влияющей на усредненеие
    static void OsciSettingsEffectOnAverageChanged();
};
