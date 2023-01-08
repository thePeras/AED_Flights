#include "Menu.h"

Menu::Menu(string title, string inputText, vector<string> options, vector<string> list, bool validateInput, bool isToPrintList, int nColumns, bool acceptIntegers) : title(title), inputText(inputText), listToPrint(list), validateInput(validateInput), isToPrintList(isToPrintList), nColumns(nColumns), acceptIntegers(acceptIntegers){
    this->listIndex = 0;
    this->options = options;
    this->optionSelected = false;
    this->listToValidate = list;
}

Menu::Menu(string title, string inputText, vector<string> options, vector<string> list, vector<string> listToValidate, bool validateInput, bool isToPrintList, int nColumns, bool acceptIntegers) : title(title), inputText(inputText), listToPrint(list), validateInput(validateInput), isToPrintList(isToPrintList), nColumns(nColumns), acceptIntegers(acceptIntegers){
    this->listIndex = 0;
    this->options = options;
    this->optionSelected = false;
    this->listToValidate = listToValidate;
}

void Menu::showList(){
    int max = 0;
    for (int i = 0; i < listToPrint.size(); i++) {
        if(listToPrint[i].size() > max) max = listToPrint[i].size();
    }

    for (int i = this->listIndex; i < this->listIndex + nLines * nColumns && i < listToPrint.size(); i++) {
        if(i % nColumns == 0) cout << endl;
        cout << '\t' << left << setw(max+3) << listToPrint[i];
    }
    cout << endl << endl;
}

bool Menu::inputIsInvalid(string input){
    try{
        int inputInt = stoi(input);
        if(acceptIntegers) return false;
        if(isToPrintList and listToPrint.size() > nLines * nColumns and inputInt >= 0 and inputInt <= options.size()) return false;
        if(isToPrintList and listToPrint.size() > nLines * nColumns and inputInt >= 0 and listIndex > 0 and inputInt <= options.size() + 1) return false;
        if(inputInt >= 0 and inputInt < options.size()) return false;
        return find(listToValidate.begin(), listToValidate.end(), input) == listToValidate.end();
    }catch (exception e){
        return find(listToValidate.begin(), listToValidate.end(), input) == listToValidate.end();
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
    if(isToPrintList && listToPrint.size() > nLines * nColumns){
        cout << "\t" << options.size() << " - Próxima página" << endl;
        if(listIndex > 0) cout << "\t" << options.size() + 1 << " - Página anterior" << endl;
    }
    cout << endl << inputText;
    getline(cin, input);
    while(inputIsInvalid(input) && validateInput) {
        cout << inputText;
        getline(cin, input);
    }

    this->input_value = input;

    try {
        int inputInt = stoi(input);
        if(inputInt >= 0 and inputInt < options.size()){
            optionSelected = true;
            return;
        }
        if(isToPrintList and listToPrint.size() > nLines * nColumns and inputInt == options.size()){
            increaseListIndex();
            render();
            return;
        }
        if(isToPrintList and listToPrint.size() > nLines * nColumns and inputInt == options.size() + 1 and listIndex > 0){
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
    if(listIndex + nColumns * nLines < listToPrint.size()) listIndex += nColumns * nLines;
}

void Menu::decreaseListIndex(){
    if(listIndex - nColumns * nLines >= 0) listIndex -= nColumns * nLines;
}

void Menu::setList(vector<string> list) {
    this->listToPrint = list;
}
