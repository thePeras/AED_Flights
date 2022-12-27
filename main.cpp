#include <iostream>
#include "MenuTwo.h"
#include <string>
#include <vector>
#include <stack>
#include "MenuOption.h"
#include "Managing.h"

using namespace std;
stack<MenuTwo> menuStack;

void back_action(){
    menuStack.pop();
    menuStack.top().render();
}

void aeroporto_input(){
    MenuTwo menu_aeroporto_input("Aeroporto", "digite o nome do aeroporto", {}, {"OPO", "JFK"});
    menu_aeroporto_input.render();
    cout << menu_aeroporto_input.getInput();
}

void menu_viajar(){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
            {"Aeroporto (TAG)", aeroporto_input},
            {"País", []() {}},
            {"Coordenadas", []() {}},
    };

    MenuTwo menu_viajar("Viajar", "de onde? ", options_viajar, {});
    menuStack.push(menu_viajar);

    menu_viajar.render();
}

int main(){
    Managing m;
    m.readFiles();

    Graph g = m.getGraph();
    // perform bfs on the graph and print the airports
    g.bfs(g.airports["OPO"]);

    vector<MenuOption> options = {
            {"Viajar", menu_viajar},
            {"Consultar Aeroporto", []() {cout << "Remove Location" << endl;}},
            {"Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    MenuTwo MainMenu("Inicio", "Escolha uma opção", options, {}, false);
    menuStack.push(MainMenu);
    MainMenu.render();



    return 0;
}