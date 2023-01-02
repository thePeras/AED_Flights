#pragma once

#include <stack>
#include "Airport.h"

class menus {
    private:
        void back_action();
        void back_and_empty_action();
        void back_action_twice();
        void exit_action();

    public:
        menus();
        void mainMenu();

        //VIAJAR
        void menu_viajar(string title);
        void aeroporto_input();
        void menu_pais();
        void menu_cidade(string country);
        void menu_cidades(string country);
        void menu_coordenadas();

        //CONSULTAR AEROPORTO
        void digitar_aeroporto();
        void consultar_aeroporto(Airport &airport);
        void voos_aeroporto(Airport airport);

        //CONSULTAR REDE
        void consultar_rede();
        void consultar_rede_global();
        void consultar_rede_companhia();

};
