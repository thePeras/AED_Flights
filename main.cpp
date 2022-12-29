#include <iostream>
#include "MenuTwo.h"
#include <string>
#include <vector>
#include <stack>
#include "MenuOption.h"
#include "Managing.h"

using namespace std;
stack<MenuTwo> menuStack;
Managing m;

void back_action(){
    menuStack.pop();
    menuStack.top().render();
}

void exit_action(){
    cout << endl << "Leaving..." << endl;
    cout << "Goodbye!" << endl;
    exit(0);
}

void aeroporto_input(){
    MenuTwo menu_aeroporto_input("Aeroporto", "digite o código do aeroporto: ", {}, {"OPO", "JFK"});
    menu_aeroporto_input.render();
    cout << menu_aeroporto_input.getInput();
}

bool latitudeIsValid(string input){
    try{
        double latitude = stod(input);
        if(latitude >= -90 and latitude <= 90) return true;
        return false;
    }catch (exception e){
       return false;
    }
}
bool longitudeIsValid(string input){
    try{
        double latitude = stod(input);
        if(latitude >= -180 and latitude <= 190) return true;
        return false;
    }catch (exception e){
        return false;
    }
}

bool radiusIsValid(string input){
    double MAX_RADIUS = 40075; // Perímetro da circunferencia do Equador
    try{
        double radius = stod(input);
        if(radius > 0 and radius <= MAX_RADIUS) return true;
        return false;
    }catch (exception e){
        return false;
    }
}

void menu_coordenadas(){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
    };

    MenuTwo menu_latitude("Viajar - De Onde?", "latitude", options_viajar);
    menuStack.push(menu_latitude);
    menu_latitude.render();
    string latitude = menu_latitude.getInput();
    while(!latitudeIsValid(latitude)){
        menu_latitude.render();
        latitude = menu_latitude.getInput();
    }

    MenuTwo menu_longitude("Viajar - De Onde?", "longitude", options_viajar);
    menu_longitude.render();
    string longitude = menu_longitude.getInput();
    while(!longitudeIsValid(longitude)){
        menu_longitude.render();
        longitude = menu_longitude.getInput();
    }

    MenuTwo menu_radius("Viajar - De Onde?", "raio", options_viajar);
    menu_radius.render();
    string radius = menu_radius.getInput();
    while(!radiusIsValid(radius)){
        menu_radius.render();
        radius = menu_radius.getInput();
    }

    vector<Airport> test = m.getAirportsInRadius(Location(stod(latitude), stod(longitude)), stod(radius));
    cout << "Aeroportos encontrados: " << test.size() << endl;
    for (int i = 0; i < test.size(); i++) {
        cout << test[i].getCode() << endl;
    }
}
void menu_cidades(){
    vector<MenuOption> options_cidades = {
            {"Voltar", back_action},
    };

    vector<string> airports = m.getAirportsInCountry(menuStack.top().getPreviousInput());

    MenuTwo menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options_cidades, airports, true, true);
    menuStack.push(menu_aeroporto);

    menu_aeroporto.render();

    Airport airport = m.getAirports().find(menu_aeroporto.getInput())->second;

    //get curiosidades

    vector<MenuOption> options2 = {
            {"Voltar",                      back_action},
            {"Informação sobre os voos",    []() {}},
            {"Ver destinos diretos",        []() {}},
            {"Ver destinos com X voos",     []() {}},
            {"ideia: Ver destinos a X kms", []() {}},
    };
    MenuTwo consultar_aeroporto("Aeroporto - " + airport.getName(), "opção: ", options2, {});
    consultar_aeroporto.render();

}


void menu_cidade() {
    vector<MenuOption> options_cidade = {
            {"Voltar", back_action},
    };

    string country = menuStack.top().getPreviousInput();

    vector<string> cities = m.getCountryCities().find(country)->second;

    MenuTwo menu_cidade("Viajar - Cidade", "Escolha uma cidade", options_cidade, cities, true, true);
    menuStack.push(menu_cidade);
    menu_cidade.render();

    string city = menu_cidade.getInput();
    vector<string> airports = m.getAirportsInCity(city, country);

    MenuTwo menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options_cidade, airports, true, true);
    menuStack.push(menu_aeroporto);
    menu_aeroporto.render();

    Airport airport = m.getAirports().find(menu_aeroporto.getInput())->second;

    //get curiosidades

    vector<MenuOption> options2 = {
            {"Voltar",                      back_action},
            {"Informação sobre os voos",    []() {}},
            {"Ver destinos diretos",        []() {}},
            {"Ver destinos com X voos",     []() {}},
            {"ideia: Ver destinos a X kms", []() {}},
    };
    MenuTwo consultar_aeroporto("Aeroporto - " + airport.getName(), "opção: ", options2, {});
    consultar_aeroporto.render();
}

void menu_pais(){
    vector<MenuOption> options_pais = {
            {"Voltar", back_action}
    };

    auto countryCities = m.getCountryCities();  //buscar os países para imprimir
    vector<string> countries;
    for(auto & countryCitie : countryCities){
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    MenuTwo menu_pais("Viajar - País", "Introduza um país", options_pais, countries, true, true);
    menuStack.push(menu_pais);

    menu_pais.render();
    string country = menu_pais.getInput();

    vector<MenuOption> options2 = {
            {"Voltar", back_action},
            {"Selecionar todas as cidades", menu_cidades},
            {"Selecionar uma cidade", menu_cidade, country}
    };

    MenuTwo menu_escolha("Viajar - Cidade", "Escolha uma opção", options2, {}, true, true);
    menuStack.push(menu_escolha);
    menu_escolha.render();

}

void menu_viajar(){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
            {"Aeroporto (TAG)", aeroporto_input},
            {"País", menu_pais},
            {"Coordenadas", menu_coordenadas},
    };

    MenuTwo menu_viajar("Viajar", "De Onde", options_viajar, {});
    menuStack.push(menu_viajar);

    menu_viajar.render();
}

void consultar_aeroporto(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };

    vector<string> aeroportos;
    for(auto it = m.getAirports().begin(); it != m.getAirports().end(); it++) {
        aeroportos.push_back(it->second.getCode());
    }


    MenuTwo digitar_aeroporto("Consultar aeroporto", "código do aeroporto: ", options, aeroportos, true, true);
    menuStack.push(digitar_aeroporto);

    digitar_aeroporto.render();

    Airport theAirport = m.getAirports().find(digitar_aeroporto.getInput())->second;

    //get curiosidades

    vector<MenuOption> options2 = {
            {"Voltar", back_action},
            {"Informação sobre os voos", []() {}},
            {"Ver destinos diretos", []() {}},
            {"Ver destinos com X voos", [](){}},
            {"ideia: Ver destinos a X kms", [](){}},
    };
    MenuTwo consultar_aeroporto("Aeroporto - " + theAirport.getName(), "opção: ", options2, {});
    consultar_aeroporto.render();
    consultar_aeroporto.render();
}

int main(){
    m.readFiles();

    Graph g = m.getGraph();
    // perform bfs on the graph and print the airports
    g.bfs(g.airports["OPO"]);

    vector<MenuOption> options = {
            {"Sair", exit_action},
            {"Viajar", menu_viajar},
            {"Consultar Aeroporto", consultar_aeroporto},
            {"Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    MenuTwo MainMenu("Inicio", "Escolha uma opção", options, {}, false);
    menuStack.push(MainMenu);
    MainMenu.render();

    return 0;
}