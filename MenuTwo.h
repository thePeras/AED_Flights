#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "MenuOption.h"
#include <iostream>

using namespace std;

template <typename T>
class MenuTwo {
private:
    const string title;
    const string inputText;
    vector<MenuOption> options;
    const vector<T> list;
    const bool backOption;
    const bool isToPrintList;
    const bool isOrderList;
    bool inputIsInvalid(string input);
public:
    MenuTwo(string title, string inputText, vector<MenuOption> options, vector<T> list, bool backOption = true, bool isToPrintList = false, bool isOrderList = false);
    void showList();
    void render();
};

template<typename T>
MenuTwo<T>::MenuTwo(string title, string inputText, vector<MenuOption> options, vector<T> list, bool backOption, bool isToPrintList, bool isOrderList) : title(title), inputText(inputText), list(list), backOption(backOption), isToPrintList(isToPrintList), isOrderList(isOrderList) {
    if(backOption) options.insert(options.begin(), MenuOption{'0', "Voltar", nullptr});
    this->options = options;
}


template<typename T>
void MenuTwo<T>::showList(){
    for (int i = 0; i < list.size(); i++) { //The i variable needs to be started at options.size() + 1
        if(isOrderList) cout << i + 1 << " - ";
        cout << list[i] << endl;
    }
    cout << endl;
}

template <typename T>
bool MenuTwo<T>::inputIsInvalid(string input){
    try{
        int inputInt = stoi(input);
        if(inputInt >= 0 and inputInt < options.size()) return true;
    }catch (exception e){
        return find(list.begin(), list.end(), input) == list.end();
    }
}

template<typename T>
void MenuTwo<T>::render(){
    string input;
    cout << "title: " << title << endl;
    if(isToPrintList) showList();
    cout << "Options: " << endl;
    for (int i = 0; i < options.size(); i++) {
        cout << options[i].choice << " - " << options[i].text << endl;
    }
    cout << inputText << ": ";
    cin >> input;
    while(inputIsInvalid(input)) {
        cout << inputText << ": ";
        cin >> input;
    }

    try {
        int inputInt = stoi(input);
        if(inputInt == 0){
            cout << "Voltar" << endl;
            return;
        }
        if(inputInt > 0 and inputInt < options.size()){
            options[inputInt].action();
            return;
        }
    }
    catch (exception e) {
        cout << "input: " << input << endl;
    }
}

