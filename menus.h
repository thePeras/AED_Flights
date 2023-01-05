#pragma once

#include "Airport.h"

class menus {
    private:
        void exit_action();

    public:
        menus();
        void mainMenu();

        //VIAJAR
        void menu_viajar(string title);
        void aeroporto_input();
        void menu_pais();
        void menus_pais_cidades(string country);
        void menu_cidade(string city, string country);
        void menu_coordenadas();

        //CONSULTAR AEROPORTO
        void digitar_aeroporto();
        void consultar_aeroporto(Airport &airport);
        void voos_aeroporto(Airport airport);

        //CONSULTAR REDE
        void consultar_rede();
        void consultar_rede_global();
        void digitar_companhia();
        void consultar_rede_companhia(string airlineCode);
        void digitar_pais();
        void consultar_rede_pais(string country);


        void menu_results();
        void menu_filtrar();
        void menu_companhias();
        void menu_escala();
};
