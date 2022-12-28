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

void pais_input(){
    MenuTwo menu_pais_input("País ", "digite o nome do país", {}, {"Portugal", "Espanha"});
    menu_pais_input.render();
    cout << menu_pais_input.getInput();
}

void coordenadas_input(){
    MenuTwo menu_coordenadas_input("Coordenadas ", "digite latitude: ", {}, {});
    MenuTwo menu_coordenadas_input2("Coordenadas ", "digite longitude: ", {}, {});
    menu_coordenadas_input.render();
    cout << menu_coordenadas_input.getInput();
}


void menu_viajar(){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
            {"Aeroporto (TAG)", aeroporto_input},
            {"País", pais_input},
            {"Coordenadas", coordenadas_input},
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
    for (auto el : g.possiblePaths("OPO", "PEK")) {
        for (auto el2 : el) {
            cout << el2 << " ";
        }
        cout << endl;
    }


    vector<MenuOption> options = {
            {"VIAJAR", menu_viajar},
            {"CONSULTAR AEROPORTO", []() {cout << "Remove Location" << endl;}},
            {"OS MEUS BILHETES", []() {cout << "Edit Location" << endl;}},
    };

    MenuTwo MainMenu("Inicio", "Escolha uma opção", options, {}, false);
    menuStack.push(MainMenu);
    MainMenu.render();



    return 0;
}