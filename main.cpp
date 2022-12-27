#include <iostream>
#include "MenuTwo.h"
#include <string>
#include <vector>
#include "MenuOption.h"

using namespace std;

int main(){
    //Menu menu = Menu();
    vector<MenuOption> options = {
        {'1', "Viajar", []() {cout << "push para o menu viajar" << endl;}},
        {'2', "Consultar Aeroporot", []() {cout << "Remove Location" << endl;}},
        {'3', "Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    vector<string> my_list = {"Lisboa", "Porto", "Coimbra", "Braga", "Faro"};
    string title = "Inicio";
    string inputText = "Escolha uma opção";
    MenuTwo<string> menu2(title, inputText, options, my_list, false);
    menu2.render();

    return 0;
}