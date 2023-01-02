#include <iostream>
#include "MenuTwo.h"
#include <string>
#include <vector>
#include <stack>
#include "MenuOption.h"
#include "Managing.h"
#include <sstream>

using namespace std;
stack<MenuTwo*> menuStack;
Managing m;

void back_action(){
    menuStack.pop();
    menuStack.top()->render();
}

void back_action_twice(){
    menuStack.pop();
    menuStack.pop();
    menuStack.top()->render();
}

void exit_action(){
    cout << endl << "Leaving..." << endl;
    cout << "Goodbye!" << endl;
    exit(0);
}

void aeroporto_input(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };
    //make a vector with all the airports
    vector<string> airports;
    for(auto airport : m.getAirports()){
        airports.push_back(airport.second.getCode());
    }
    sort(airports.begin(), airports.end());

    MenuTwo menu_aeroporto_input("Aeroporto", "digite o código do aeroporto: ", options,airports, true, true);
    menu_aeroporto_input.render();

    //se chegar aqui significa que escolheu um aeroporto
    //TODO: fazer os flights
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
    menuStack.push(&menu_latitude);
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

void menu_cidades() {
    vector<MenuOption> options = {
            {"Voltar", back_action},

    };

    string country = menuStack.top()->getSavedVariable();
    vector<string> airports = m.getAirportsInCountry(country);

    MenuTwo menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options, airports, true, true);
    menuStack.push(&menu_aeroporto);

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

void menu_cidade(){
    vector<MenuOption> options_cidade = {
            {"Voltar", back_action},
    };

    string country = menuStack.top()->getSavedVariable();
    vector<string> cities = m.getCountryCities().find(country)->second;

    MenuTwo menu_cidade("Viajar - Cidade", "Escolha uma cidade", options_cidade, cities, true, true);
    menuStack.push(&menu_cidade);
    menu_cidade.render();

    string city = menu_cidade.getInput();
    vector<string> airports = m.getAirportsInCity(city, country);


    vector<MenuOption> options_aeroporto = {
            {"Voltar", back_action_twice},
    };

    MenuTwo menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options_aeroporto, airports, true, true);
    menuStack.push(&menu_aeroporto);
    menu_aeroporto.render();

    Airport airport = m.getAirports().find(menu_aeroporto.getInput())->second;

    //get curiosidades

    vector<MenuOption> options2 = {
            {"Voltar",                      back_action_twice},
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

    auto countryCities = m.getCountryCities();  //get countries
    vector<string> countries;
    for(auto & countryCitie : countryCities){
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    MenuTwo menu_pais("Viajar - País", "Introduza um país", options_pais, countries, true, true);
    menuStack.push(&menu_pais);

    menu_pais.render();

    string country = menu_pais.getInput();

    vector<MenuOption> options2 = {
            {"Voltar", back_action_twice},
            {"Selecionar todas as cidades", menu_cidades},
            {"Selecionar uma cidade", menu_cidade}

    };

    MenuTwo menu_escolha("Viajar - " + country, "Escolha uma opção", options2, {}, true, true);
    menuStack.push(&menu_escolha);
    menu_escolha.setSavedVariable(country);
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
    menuStack.push(&menu_viajar);

    menu_viajar.render();
}

void voos_aeroporto(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };
    Airport airport = m.getAirports().find(menuStack.top()->getSavedVariable())->second;

    vector<string> flights;
    for (auto & flight : airport.getFlights()) {
        flights.push_back(flight->getTarget());
    }

    MenuTwo menu_voos_aeroporto("Voos - Aeroporto", "opção", options, flights, false, true);
    menuStack.push(&menu_voos_aeroporto);

    menu_voos_aeroporto.render();
}

void consultar_aeroporto(Airport airport){
    vector<MenuOption> options = {
            {"Voltar ao menu principal", back_action_twice},
            {"Informação sobre os voos", voos_aeroporto},
            {"Ver destinos diretos", []() {}},
            {"Ver destinos com X voos", [](){}},
            {"ideia: Ver destinos a X kms", [](){}},
    };

    stringstream curiosity1;
    curiosity1 << "Neste aeroporto descola";
    if(airport.getFlights().size() != 1) curiosity1 << "m";
    if(airport.getFlights().size() < 10) curiosity1 << " apenas";
    curiosity1 << " " << airport.getFlights().size() << " voo";
    if(airport.getFlights().size() != 1) curiosity1 << "s";

    stringstream curiosity2;
    curiosity2 << "Operam " << airport.getAirlines().size() << " companhias areas";

    stringstream curiosity3;
    pair<string, int> mostDistanceCountry = m.mostDistantCountry(airport.getCode(), 2);
    curiosity3 << "Com " << mostDistanceCountry.second << " voo";
    if(mostDistanceCountry.second != 1) curiosity3 << "s";
    curiosity3 << " consegues ir ao país " << mostDistanceCountry.first;

    vector<string> curiosities = {curiosity1.str(), curiosity2.str(), curiosity3.str()};

    string airportName = " (" + airport.getCode() + ") Aeroporto - " + airport.getName() + ", " + airport.getCountry();

    MenuTwo consultar_aeroporto(airportName, "Opção", options, curiosities, true, true);
    consultar_aeroporto.setSavedVariable(airport.getCode());
    menuStack.push(&consultar_aeroporto);
    consultar_aeroporto.render();
}

void digitar_aeroporto(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };

    vector<string> aeroportos;
    for(auto it = m.getAirports().begin(); it != m.getAirports().end(); it++) {
        aeroportos.push_back(it->second.getCode());
    }

    sort(aeroportos.begin(), aeroportos.end());

    MenuTwo digitar_aeroporto("Consultar aeroporto", "código do aeroporto: ", options, aeroportos, true, true);
    menuStack.push(&digitar_aeroporto);
    digitar_aeroporto.render();

    Airport theAirport = m.getAirports().find(digitar_aeroporto.getInput())->second;

    consultar_aeroporto(theAirport);

}


void consultar_rede_companhia(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };

    vector<string> airlines;
    for(auto it = m.getAirlines().begin(); it != m.getAirlines().end(); it++) {
        airlines.push_back(it->second.getCode());
    }

    MenuTwo consultar_rede_companhia("Consultar rede de uma companhia", "companhia", options, airlines, true, true);
    menuStack.push(&consultar_rede_companhia);
    consultar_rede_companhia.render();

    string airlineCode = consultar_rede_companhia.getInput();
    Airline airline = m.getAirlines().find(airlineCode)->second;
    unordered_map<string, Airport> network = m.getUndirectedAirlineNetwork(airlineCode);
    set<string> articulationPoints = m.getArticulationPoints(network, airline.getFlights()[0]->getSource());

    stringstream line1;
    line1 << "Esta companhia area tem " << articulationPoints.size() << " aeroportos importantes (Pontos de articulação)." << endl;

    vector<string> text = {line1.str()};

    vector<MenuOption> options2 = {
            {"Voltar", back_action_twice},
    };

    MenuTwo consultar_rede_unica("Rede " + airline.getName(), "opção", options2, text, false, true);
    menuStack.push(&consultar_rede_unica);
    consultar_rede_unica.render();
}

void consultar_rede_global(){
    unordered_map<string, Airport> network = m.getUndirectedGlobalNetwork();

    set<string> articulationPoints = m.getArticulationPoints(network, "OPO");

    stringstream line1;
    line1 << "Existem " << articulationPoints.size() << " aeroportos importantes (Pontos de articulação)." << endl;

    vector<string> text = {line1.str()};

    vector<MenuOption> options = {
            {"Voltar", back_action},
    };

    MenuTwo consultar_rede_global("Consultar rede global", "opção", options, text, false, true);
    menuStack.push(&consultar_rede_global);
    consultar_rede_global.render();
}
void consultar_rede(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
            {"Rede Global", consultar_rede_global},
            {"Rede de uma companhia aerea", consultar_rede_companhia},
    };

    MenuTwo consultar_rede("Consultar rede", "opção", options, {});
    menuStack.push(&consultar_rede);
    consultar_rede.render();
}

int main(){
    m.readFiles();

    // TODO: passar para o menu das redes
    //cout << endl << "Existem, ao todo, " << m.getAirports().size() << " aeroportos.";
    //cout << "A rede global possui " << m.getAirports().size() << " aeroportos e tem um diâmetro de " << m.getDiameter(m.getAirports()) << endl;
    vector<MenuOption> options = {
            {"Sair", exit_action},
            {"Viajar", menu_viajar},
            {"Consultar Aeroporto", digitar_aeroporto},
            {"Consultar Rede", consultar_rede},
            {"Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    MenuTwo MainMenu("Inicio", "Escolha uma opção", options, {}, true);
    menuStack.push(&MainMenu);
    MainMenu.render();

    return 0;
}