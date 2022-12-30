#include "menus.h"
#include <iostream>
#include "MenuOption.h"
#include "Managing.h"
#include "Menu.h"

using namespace std;

Managing m;

menus::menus() {

}

void menus::digitar_aeroporto(){
    vector<MenuOption> options = {
           // {"Voltar", back_action},
    };

    vector<string> aeroportos;
    for(auto it = m.getAirports().begin(); it != m.getAirports().end(); it++) {
        aeroportos.push_back(it->second.getCode());
    }

    sort(aeroportos.begin(), aeroportos.end());

    Menu digitar_aeroporto("Consultar aeroporto", "código do aeroporto: ", options, aeroportos, true, true);
    //menuStack.push(&digitar_aeroporto);
    digitar_aeroporto.render();

    //Airport theAirport = ma.getAirports().find(digitar_aeroporto.getInput())->second;

    //consultar_aeroporto(theAirport);
}
