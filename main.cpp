#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "MenuOption.h"
#include "Managing.h"
#include <sstream>
#include "menus.h"

using namespace std;
stack<Menu*> menuStack;
Managing ma;
vector<Airport> source_airports;
vector<Airport> target_airports;

menus menus;

void back_action(){
    menuStack.pop();
    menuStack.top()->render();
}

void back_action_empty(){
    source_airports.clear();
    target_airports.clear();
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
    for(auto airport : ma.getAirports()){
        airports.push_back(airport.second.getCode());
    }
    sort(airports.begin(), airports.end());

    Menu menu_aeroporto_input("Aeroporto", "digite o código do aeroporto: ", options, airports, true, true);
    menu_aeroporto_input.render();

    string input = menu_aeroporto_input.getInput();
    Airport airport = ma.getAirports().at(input);

    if(source_airports.empty()){ //estou no viajar
        source_airports.push_back(airport);
        //menu_viajar("Para onde?");
    }else{ //estou no destino
        target_airports.push_back(airport);
        //TODO: run the algorithm
    }
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

    Menu menu_latitude("Viajar - De Onde?", "latitude", options_viajar);
    menuStack.push(&menu_latitude);
    menu_latitude.render();
    string latitude = menu_latitude.getInput();
    while(!latitudeIsValid(latitude)){
        menu_latitude.render();
        latitude = menu_latitude.getInput();
    }

    string longitude;
    cout << "longitude: ";
    cin >> longitude;
    while(!longitudeIsValid(longitude)){
        cout << "longitude: ";
        cin >> longitude;
    }

    string radius;
    cout << "raio: ";
    cin >> radius;
    while(!radiusIsValid(radius)){
        cout << "raio: ";
        cin >> radius;
    }

    vector<Airport> founded_airports = ma.getAirportsInRadius(Location(stod(latitude), stod(longitude)), stod(radius));

    if(source_airports.empty()){
        source_airports = founded_airports;
        //menu_viajar("Para onde?");
    }else{
        target_airports = founded_airports;
        //TODO: run the algorithm
    }
}

void menu_cidades() {
    vector<MenuOption> options = {
            {"Voltar", back_action},

    };

    string country = menuStack.top()->getSavedVariable();
    vector<string> airports = ma.getAirportsInCountry(country);

    Menu menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options, airports, true, true);
    menuStack.push(&menu_aeroporto);

    menu_aeroporto.render();

    Airport airport = ma.getAirports().find(menu_aeroporto.getInput())->second;

    //get curiosidades

    vector<MenuOption> options2 = {
            {"Voltar",                      back_action},
            {"Informação sobre os voos",    []() {}},
            {"Ver destinos diretos",        []() {}},
            {"Ver destinos com X voos",     []() {}},
            {"ideia: Ver destinos a X kms", []() {}},
    };
    Menu consultar_aeroporto("Aeroporto - " + airport.getName(), "opção: ", options2, {});
    consultar_aeroporto.render();
}

void menu_cidade(){
    vector<MenuOption> options_cidade = {
            {"Voltar", back_action},
    };

    string country = menuStack.top()->getSavedVariable();
    vector<string> cities = ma.getCountryCities().find(country)->second;

    Menu menu_cidade("Viajar - Cidade", "Escolha uma cidade", options_cidade, cities, true, true);
    menuStack.push(&menu_cidade);
    menu_cidade.render();

    string city = menu_cidade.getInput();
    vector<string> airports = ma.getAirportsInCity(city, country);


    vector<MenuOption> options_aeroporto = {
            {"Voltar", back_action_twice},
    };

    Menu menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", options_aeroporto, airports, true, true);
    menuStack.push(&menu_aeroporto);
    menu_aeroporto.render();

    Airport airport = ma.getAirports().find(menu_aeroporto.getInput())->second;

    //get curiosidades


    vector<MenuOption> options2 = {
            {"Voltar",                      back_action_twice},
            {"Informação sobre os voos",    []() {}},
            {"Ver destinos diretos",        []() {}},
            {"Ver destinos com X voos",     []() {}},
            {"ideia: Ver destinos a X kms", []() {}},
    };
    Menu consultar_aeroporto("Aeroporto - " + airport.getName(), "opção: ", options2, {});
    consultar_aeroporto.render();
}


void menu_pais(){
    vector<MenuOption> options_pais = {
            {"Voltar", back_action}
    };

    auto countryCities = ma.getCountryCities();  //get countries
    vector<string> countries;
    for(auto & countryCitie : countryCities){
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    Menu menu_pais("Viajar - País", "Introduza um país", options_pais, countries, true, true);
    menuStack.push(&menu_pais);

    menu_pais.render();

    string country = menu_pais.getInput();

    vector<MenuOption> options2 = {
            {"Voltar", back_action_twice},
            {"Selecionar todas as cidades", menu_cidades},
            {"Selecionar uma cidade", menu_cidade}

    };

    Menu menu_escolha("Viajar - " + country, "Escolha uma opção", options2, {}, true, true);
    menuStack.push(&menu_escolha);
    menu_escolha.setSavedVariable(country);
    menu_escolha.render();
}

void menu_viajar(string title){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
            {"Aeroporto (TAG)", aeroporto_input},
            {"País", menu_pais},
            {"Coordenadas", menu_coordenadas},
    };

    Menu menu_viajar("Viajar", title, options_viajar, {});
    menuStack.push(&menu_viajar);

    menu_viajar.render();
}

void voos_aeroporto(){
    vector<MenuOption> options = {
            {"Voltar", back_action},
    };
    Airport airport = ma.getAirports().find(menuStack.top()->getSavedVariable())->second;

    vector<string> flights;
    for (auto & flight : airport.getFlights()) {
        flights.push_back(flight->getTarget());
    }

    Menu menu_voos_aeroporto("Voos - Aeroporto", "opção", options, flights, false, true);
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
    pair<string, int> mostDistanceCountry = ma.mostDistantCountry(airport.getCode(), 2);
    curiosity3 << "Com " << mostDistanceCountry.second << " voo";
    if(mostDistanceCountry.second != 1) curiosity3 << "s";
    curiosity3 << " consegues ir ao país " << mostDistanceCountry.first;

    vector<string> curiosities = {curiosity1.str(), curiosity2.str(), curiosity3.str()};

    string airportName = " (" + airport.getCode() + ") Aeroporto - " + airport.getName() + ", " + airport.getCountry();

    Menu consultar_aeroporto(airportName, "Opção", options, curiosities, true, true);
    consultar_aeroporto.setSavedVariable(airport.getCode());
    menuStack.push(&consultar_aeroporto);
    consultar_aeroporto.render();
}



int main(){
    ma.readFiles();
    
    vector<MenuOption> options = {
            {"Sair", exit_action},
            {"Viajar", [](){menu_viajar("De onde?");}},
            {"Consultar Aeroporto", menus.digitar_aeroporto},
            {"Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    Menu MainMenu("Inicio", "Escolha uma opção", options, {}, true);
    menuStack.push(&MainMenu);
    MainMenu.render();

    return 0;
}