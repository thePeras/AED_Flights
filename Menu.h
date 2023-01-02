#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Menu {
private:
    const string title;
    const string inputText;
    vector<string> options;
    const vector<string> list;
    int listIndex;
    const bool isToPrintList;
    const bool isOrderList;
    string input_value;
    bool inputIsInvalid(string input);
    bool validateInput;
    bool optionSelected;
    void increaseListIndex();
    void decreaseListIndex();
public:
    Menu(string title, string inputText, vector<string> options, vector<string> list = {}, bool validateInput = false, bool isToPrintList = false, bool isOrderList = false);
    void showList();
    void render();
    string getInput();
    bool optionIsSelected();
    int getOption();
};

Menu::Menu(string title, string inputText, vector<string> options, vector<string> list, bool validateInput, bool isToPrintList, bool isOrderList) : title(title), inputText(inputText), list(list), validateInput(validateInput), isToPrintList(isToPrintList), isOrderList(isOrderList) {
    this->listIndex = 0;
    this->options = options;
    this->optionSelected = false;
}

void Menu::showList(){
    for (int i = this->listIndex; i < this->listIndex + 10 && i < list.size(); i++) {
        if(isOrderList) cout << i + 1 << " - ";
        cout << list[i] << endl;
    }
    cout << endl;
}

bool Menu::inputIsInvalid(string input){
    try{
        int inputInt = stoi(input);
        if(isToPrintList and list.size() > 10 and inputInt >= 0 and inputInt <= options.size()) return false;
        if(isToPrintList and list.size() > 10 and inputInt >= 0 and listIndex > 0 and inputInt <= options.size() + 1) return false;
        if(inputInt >= 0 and inputInt < options.size()) return false;
    }catch (exception e){
        return find(list.begin(), list.end(), input) == list.end();
    }
    return true;
}

void Menu::render(){
    string input;
    cout << endl << "-------- " << title << " --------" << endl << endl;
    if(isToPrintList) showList();
    cout << "Opções: " << endl;
    for (int i = 0; i < options.size(); i++) {
        cout << '\t' << i << " - " << options[i] << endl;
    }
    if(isToPrintList && list.size() > 10){
        cout << "\t" << options.size() << " - Próxima página" << endl;
        if(listIndex > 0) cout << "\t" << options.size() + 1 << " - Página anterior" << endl;
    }
    cout << endl << inputText << ": ";
    getline(cin, input);
    while(inputIsInvalid(input) && validateInput) {
        cout << inputText << ": ";
        getline(cin, input);
    }

    this->input_value = input;

    try {
        int inputInt = stoi(input);
        if(inputInt >= 0 and inputInt < options.size()){
            optionSelected = true;
            return;
        }
        if(isToPrintList and list.size() > 10 and inputInt == options.size()){
            increaseListIndex();
            render();
            return;
        }
        if(isToPrintList and list.size() > 10 and inputInt == options.size() + 1 and listIndex > 0){
            decreaseListIndex();
            render();
            return;
        }
    }
    catch (exception e) {
        this->input_value = input;
    }
}

string Menu::getInput(){
    return this->input_value;
}

int Menu::getOption(){
    return stoi(this->input_value);
}

bool Menu::optionIsSelected(){
    return this->optionSelected;
}

void Menu::increaseListIndex(){
    if(listIndex + 10 < list.size()) listIndex += 10;
}

void Menu::decreaseListIndex(){
    if(listIndex - 10 >= 0) listIndex -= 10;
}
