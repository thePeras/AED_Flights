#include "Menu.h"

Menu::Menu(string title, string inputText, vector<string> options, vector<string> list, bool validateInput, bool isToPrintList, int nColumns) : title(title), inputText(inputText), list(list), validateInput(validateInput), isToPrintList(isToPrintList), nColumns(nColumns) {
    this->listIndex = 0;
    this->options = options;
    this->optionSelected = false;
}

void Menu::showList(){
    int max = 0;
    for (int i = 0; i < list.size(); i++) {
        if(list[i].size() > max) max = list[i].size();
    }

    for (int i = this->listIndex; i < this->listIndex + nLines * nColumns && i < list.size(); i++) {
        if(i % nColumns == 0) cout << endl;
        cout << '\t' << left << setw(max+3) << list[i];
    }
    cout << endl << endl;
}

bool Menu::inputIsInvalid(string input){
    try{
        int inputInt = stoi(input);
        if(isToPrintList and list.size() > nLines * nColumns and inputInt >= 0 and inputInt <= options.size()) return false;
        if(isToPrintList and list.size() > nLines * nColumns and inputInt >= 0 and listIndex > 0 and inputInt <= options.size() + 1) return false;
        if(inputInt >= 0 and inputInt < options.size()) return false;
        return find(list.begin(), list.end(), input) == list.end();
    }catch (exception e){
        return find(list.begin(), list.end(), input) == list.end();
    }
}

void Menu::render(){
    string input;
    cout << endl << "-------- " << title << " --------" << endl;
    if(isToPrintList) showList();
    cout << "Opções: " << endl;
    for (int i = 0; i < options.size(); i++) {
        cout << '\t' << i << " - " << options[i] << endl;
    }
    if(isToPrintList && list.size() > nLines * nColumns){
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
        if(isToPrintList and list.size() > nLines * nColumns and inputInt == options.size()){
            increaseListIndex();
            render();
            return;
        }
        if(isToPrintList and list.size() > nLines * nColumns and inputInt == options.size() + 1 and listIndex > 0){
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
    if(listIndex + nColumns * nLines < list.size()) listIndex += nColumns * nLines;
}

void Menu::decreaseListIndex(){
    if(listIndex - nColumns * nLines >= 0) listIndex -= nColumns * nLines;
}