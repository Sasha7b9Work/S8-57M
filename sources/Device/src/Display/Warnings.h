// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


struct WarningStruct
{
    WarningStruct(pchar msg = nullptr);
    WarningStruct(const WarningStruct &s);
    ~WarningStruct();
    String      message;
    uint        timeStart;
    bool IsDead() const;
    int Height(int width) const;
    void Draw(int x, int y) const;
    WarningStruct &operator=(const WarningStruct &s);
};


namespace Warnings
{
    void AddWarning(pchar warning);

    void Draw();

    bool IsShown();
};
