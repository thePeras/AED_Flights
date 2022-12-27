#pragma once

typedef void (*ActionFunctionPointer)(void);

struct MenuOption
{
    char choice;
    char const * text;
    ActionFunctionPointer action;
};

