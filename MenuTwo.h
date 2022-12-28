#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "MenuOption.h"
#include <iostream>

using namespace std;

class MenuTwo {
private:
    const string title;
    const string inputText;
    vector<MenuOption> options;
    const vector<string> list;
    const bool isToPrintList;
    const bool isOrderList;
    string input_value;
    bool inputIsInvalid(string input);
    bool validateInput;
public:
    MenuTwo(string title, string inputText, vector<MenuOption> options, vector<string> list = {}, bool validateInput = false, bool isToPrintList = false, bool isOrderList = false);
    void showList();
    void render();
    string getInput();
};

MenuTwo::MenuTwo(string title, string inputText, vector<MenuOption> options, vector<string> list, bool validateInput, bool isToPrintList, bool isOrderList) : title(title), inputText(inputText), list(list), options(options), validateInput(validateInput), isToPrintList(isToPrintList), isOrderList(isOrderList) {}

void MenuTwo::showList(){
    for (int i = 0; i < list.size(); i++) {
        if(isOrderList) cout << i + 1 << " - ";
        cout << list[i] << endl;
    }
    cout << endl;
}

bool MenuTwo::inputIsInvalid(string input){
    try{
        int inputInt = stoi(input);
        if(inputInt >= 0 and inputInt <= options.size()) return false;
    }catch (exception e){
        return find(list.begin(), list.end(), input) == list.end();
    }
    return false;
}

void MenuTwo::render(){
    string input;
    cout << endl << "-------- " << title << " --------" << endl << endl;
    if(isToPrintList) showList();
    cout << "Opções: " << endl;
    for (int i = 0; i < options.size(); i++) {
        cout << '\t' << i << " - " << options[i].text << endl;
    }
    cout << endl << inputText << ": ";
    cin >> input;
    while(inputIsInvalid(input) && validateInput) {
        cout << inputText << ": ";
        cin >> input;
    }

    this->input_value = input;

    try {
        int inputInt = stoi(input);
        if(inputInt >= 0 and inputInt <= options.size()){
            options[inputInt].action();
            return;
        }
    }
    catch (exception e) {
        this->input_value = input;
    }
}

string MenuTwo::getInput(){
    return this->input_value;
}
