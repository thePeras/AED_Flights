#pragma once

typedef void (*ActionFunctionPointerWithParameter)(Airport*);
typedef void (*ActionFunctionPointer)(void);
typedef void (*ActionFunctionPointerWithParameterAndReturn)(void*, void*);
typedef void (*ActionFunctionPointerWithReturn)(void*);
typedef void (*ActionFunctionPointerWithParameterAndReturnAndParameter)(void*, void*, void*);

struct MenuOption
{
    char const * text;
    ActionFunctionPointerWithParameter action;
};

