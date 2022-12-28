#include "menu.h"
#include <iostream>
using namespace std;

Managing managing;

Menu::Menu() {
    menuState.push(MAIN_MENU);
    managing.readFiles();
    getMenu();

}

void Menu::getMenu() {
    if(!menuState.empty()){
        switch (menuState.top()) {
            case MAIN_MENU:
                mainMenu();
                break;
            case CONSULTAS_MENU: {
                auto airlines = managing.getAirlines();
                for (auto it = airlines.begin(); it != airlines.end(); it++) {
                    cout << it->second.getCode() << " - " << it->second.getName() << endl;
                }
            }
                break;
            case MODIFICACOES_MENU:

                break;
            case PEDIDOS_MENU:

                break;
        }
    }
}

void Menu::mainMenu() {

    cout << endl << "------------------------------------------------\n";
    cout << "| DEI - Departamento de Engenharia Informática |" << endl;
    cout << "|\t\t\t\t" << "Aeroportos - FEUP              |" << endl;
    cout << "------------------------------------------------\n";

    cout << "|\t" << "[1] CONSULTAS                              |\n";
    cout << "|\t" << "[2] MODIFICAR TURMAS DE ESTUDANTES         |\n";
    cout << "|\t" << "[3] GESTÃO DE PEDIDOS                      |\n";

    cout << "|                                              |";
    cout << "\n|\t" << "[0] Exit                                   |" << endl;
    cout << "------------------------------------------------" << endl;

    int escolha;
    cout << "\t" << "Escolha: ";
    cin >> escolha;

    while (!cin.good() || escolha < 0 || escolha > 3) {
        cin.clear();
        cin.ignore();

        cout << endl << "\t" << "Erro! Introduza um número válido\n";
        cout << "\t" << "Escolha: ";

        cin >> escolha;

    }
    cin.ignore();

    switch (escolha) {
        case 0:
            menuState.pop();
            break;
        case 1:
            menuState.push(CONSULTAS_MENU);
            break;
        case 2:
            menuState.push(MODIFICACOES_MENU);
            break;
        case 3:
            menuState.push(PEDIDOS_MENU);
            break;
    }
    getMenu();
}
