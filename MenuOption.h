#pragma once

typedef void (*ActionFunctionPointer)(void);

struct MenuOption
{
    char const * text;
    ActionFunctionPointer action;
    std::string input;
};

