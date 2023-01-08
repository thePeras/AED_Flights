#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

class Menu {
private:
    const int nLines = 8;
    int nColumns = 4;

    const string title;
    const string inputText;
    vector<string> options;
    vector<string> listToPrint;
    vector<string> listToValidate;
    int listIndex;
    const bool isToPrintList;
    string input_value;

    bool inputIsInvalid(string input);

    bool validateInput;
    bool acceptIntegers;
    bool optionSelected;

    void increaseListIndex();

    void decreaseListIndex();

public:
    Menu(string title, string inputText, vector<string> options, vector<string> list = {}, bool validateInput = false,
         bool isToPrintList = false, int nColumns = 4, bool acceptIntegers = false);

    Menu(string title, string inputText, vector<string> options, vector<string> list, vector<string> listToValidate,
         bool validateInput = false, bool isToPrintList = false, int nColumns = 4, bool acceptIntegers = false);

    void showList();

    void render();

    string getInput();

    bool optionIsSelected();

    int getOption();

    void setList(vector<string> vector1);
};

