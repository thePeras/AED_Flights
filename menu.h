#ifndef AED2_MENU_H
#define AED2_MENU_H


#pragma once
#include <stack>
#include <algorithm>
#include "Managing.h"


#define MAIN_MENU 0
#define CONSULTAS_MENU 1     //TODO: Mudar nomes para este trabalho
#define MODIFICACOES_MENU 2
#define PEDIDOS_MENU 3
class Menu {
public:
    Menu();

    /**
     *@brief
     * Imprime o menu inicial. Fica à espera de input do utilizador. O(1).
     */
    void mainMenu();

private:
    /**
     * @brief
     * Atualiza o menu, consoante o menu que se encontra no topo da stack. O(1).
     */
    void getMenu();

    std::stack<short int> menuState;

};


#endif //AED2_MENU_H
