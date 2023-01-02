#include "menus.h"
#include <iostream>
#include "Managing.h"
#include "Menu.h"
#include <string>
#include <vector>
#include <stack>
#include <sstream>

#include "Validate.h"

using namespace std;

Managing m;
vector<Airport> travel_source_airports;
vector<Airport> travel_target_airports;

//ACTIONS

void menus::exit_action(){
    cout << endl << "Leaving..." << endl;
    cout << "Goodbye!" << endl;
    exit(0);
}

// MENUS

// MAIN MENU

menus::menus(){
    m.readFiles();
    mainMenu();
}

void menus::mainMenu(){
    // TODO: passar para o menu das redes
    //cout << endl << "Existem, ao todo, " << m.getAirports().size() << " aeroportos.";
    //cout << "A rede global possui " << m.getAirports().size() << " aeroportos e tem um diâmetro de " << m.getDiameter(m.getAirports()) << endl;

    set<string> airlines = {"TAP", "EZY"};
    for (auto el : m.possiblePaths("OPO", "GVA", 2, airlines)) {
        for (auto el2 : el) {
            cout << el2->getSource() << " -> " << el2->getTarget() << " | " << el2->getAirline() << endl;
        }
        cout << endl;
    }

    vector<string> options = {
            "Sair",
            "Viajar",
            "Consultar Aeroporto",
            "Consultar Rede",
            "Os meus bilhetes"
    };

    Menu MainMenu("Inicio", "Escolha uma opção", options, {}, true);
    MainMenu.render();

    if(MainMenu.optionIsSelected()){
        switch (MainMenu.getOption()) {
            case 0: exit_action(); break;
            case 1: menu_viajar("De onde?"); break;
            case 2: digitar_aeroporto(); break;
            case 3: consultar_rede(); break;
            case 4: cout << "Os meus bilhetes ainda por fazer" << endl; break;
        }
    }
}

// CONSULTAR AEROPORTO MENU

void menus::digitar_aeroporto(){
    vector<string> options = { "Voltar" };

    vector<string> airports;
    for(auto it = m.getAirports().begin(); it != m.getAirports().end(); it++) {
        airports.push_back(it->second.getCode());
    }

    sort(airports.begin(), airports.end());

    Menu digitar_aeroporto("Consultar aeroporto", "código do aeroporto: ", options, airports, true, true);
    digitar_aeroporto.render();

    if(digitar_aeroporto.optionIsSelected() && digitar_aeroporto.getOption() == 0){
       mainMenu();
    }

    Airport theAirport = m.getAirports().find(digitar_aeroporto.getInput())->second;
    consultar_aeroporto(theAirport);
}

void menus::consultar_aeroporto(Airport& airport){

    vector<string> options = {
            "Voltar",
            "Informação sobre os voos",
            "Ver destinos diretos",
            "Ver destinos com X voos",
            "ideia: Ver destinos a X kms"
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

    Menu consultar_aeroporto(airportName, "Opção", options, curiosities, true, true);
    consultar_aeroporto.render();

    if(consultar_aeroporto.optionIsSelected()){
        switch (consultar_aeroporto.getOption()) {
            case 0: mainMenu(); break;
            case 1: voos_aeroporto(airport); break;
            case 2: cout << "Ver destinos diretos ainda por fazer" << endl; break;
            case 3: cout << "Ver destinos com X voos ainda por fazer" << endl; break;
            case 4: cout << "Ver destinos a X kms ainda por fazer" << endl; break;
        }
    }
}

void menus::voos_aeroporto(Airport airport){
    vector<string> options = {"Voltar"};

    vector<string> flights;
    for (auto & flight : airport.getFlights()) {
        flights.push_back(flight->getTarget());
    }

    Menu menu_voos_aeroporto("Voos - Aeroporto", "opção", options, flights, false, true);
    menu_voos_aeroporto.render();

    if(menu_voos_aeroporto.optionIsSelected() && menu_voos_aeroporto.getOption() == 0){
        consultar_aeroporto(airport);
    }
}

// VIAJAR MENU

void menus::menu_viajar(string title){
    vector<string> options = {
            "Voltar",
            "Aeroporto (TAG)",
            "País",
            "Coordenadas"
    };

    Menu menu_viajar("Viajar - " + title, title, options, {});
    menu_viajar.render();

    if(menu_viajar.optionIsSelected()){
        switch (menu_viajar.getOption()) {
            case 0: mainMenu(); break;
            case 1: aeroporto_input(); break;
            case 2: menu_pais(); break;
            case 3: menu_coordenadas(); break;
        }
    }
}

void menus::aeroporto_input(){
    vector<string> options = {"Voltar"};

    vector<string> airports;
    for(auto airport : m.getAirports()){
        airports.push_back(airport.second.getCode());
    }
    sort(airports.begin(), airports.end());

    Menu menu_aeroporto_input("Aeroporto", "digite o código do aeroporto: ", options, airports, true, true);
    menu_aeroporto_input.render();

    if(menu_aeroporto_input.optionIsSelected() && menu_aeroporto_input.getOption() == 0){
        string title = travel_source_airports.size() == 0 ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string input = menu_aeroporto_input.getInput();
    Airport airport = m.getAirports().at(input);

    //TODO: Run the algorithm
}

void menus::menu_pais(){
    vector<string> options = { "Voltar"};

    auto countryCities = m.getCountryCities();  //get countries
    vector<string> countries;
    for(auto & countryCitie : countryCities){
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    Menu country_menu("Viajar - País", "Introduza um país", options, countries, true, true);
    country_menu.render();

    if(country_menu.optionIsSelected() && country_menu.getOption() == 0){
        string title = travel_source_airports.size() == 0 ? "De onde?" : "Para onde?";
        menu_viajar(title);
        return;
    }

    string country = country_menu.getInput();
    menus_pais_cidades(country);
}

void menus::menus_pais_cidades(string country){
    vector<string> cities = m.getCountryCities().find(country)->second;

    vector<string> options_cities = {
            "Voltar",
            "Selecionar todas as cidades"
    };

    Menu menu_escolha("Viajar - " + country, "Escolha uma opção", options_cities, cities, true, true);
    menu_escolha.render();

    if(menu_escolha.optionIsSelected()){
        switch (menu_escolha.getOption()) {
            case 0: menu_pais(); return;
            case 1: {
                vector<string> airports = m.getAirportsInCountry(country);
                //TODO: Run the algorithm
                return;
            }
        }
    }

    //selected a city
    string city = menu_escolha.getInput();
    menu_cidade(city, country);
}

void menus::menu_cidade(string city, string country){
    vector<string> airports = m.getAirportsInCity(city, country);

    if(airports.size() == 1){
        Airport airport = m.getAirports().find(airports[0])->second;
        //TODO: run the algorithm
    }

    vector<string> airport_options = {
            "Voltar",
            "Selecionar todos os aeroportos",
    };

    Menu menu_aeroporto("Viajar - Aeroporto", "Código do aeroporto", airport_options, airports, true, true);
    menu_aeroporto.render();

    if(menu_aeroporto.optionIsSelected()){
        switch (menu_aeroporto.getOption()) {
            case 0: menus_pais_cidades(country); return;
            case 1: {
                //TODO: run the algorithm with aiports vector
                return;
            }
        }
    }

    string airport = menu_aeroporto.getInput();
    Airport a = m.getAirports().find(airport)->second;

    //TODO: run the algorithm with a
}

void menus::menu_coordenadas(){
    vector<string> options_viajar = {"Voltar"};

    Menu menu_latitude("Viajar", "latitude", options_viajar);
    menu_latitude.render();

    if(menu_latitude.optionIsSelected() && menu_latitude.getOption() == 0){
        string title = travel_source_airports.size() == 0 ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string latitude = menu_latitude.getInput();
    do{
        latitude = menu_latitude.getInput();
    }while(!Validate::latitude(latitude));

    string longitude;
    do{
        cout << "longitude: ";
        cin >> longitude;
    }while(!Validate::longitude(longitude));

    string radius;
    do{
        cout << "raio: ";
        cin >> radius;
    }while(!Validate::radius(radius));


    vector<Airport> founded_airports = m.getAirportsInRadius(Location(stod(latitude), stod(longitude)), stod(radius));

    //TODO: Run the algorithm
}

// CONSULTAR REDE MENU

void menus::consultar_rede(){
    vector<string> options = {
            "Voltar",
            "Rede Global",
            "Rede de uma companhia"
    };

    Menu consultar_rede("Consultar rede", "opção", options, {});
    consultar_rede.render();

    if(consultar_rede.optionIsSelected()){
        switch (consultar_rede.getOption()) {
            case 0: mainMenu(); break;
            case 1: consultar_rede_global(); break;
            case 2: consultar_rede_companhia(); break;
        }
    }
}

void menus::consultar_rede_global(){
    unordered_map<string, Airport> network = m.getUndirectedGlobalNetwork();

    set<string> articulationPoints = m.getArticulationPoints(network, "OPO");

    stringstream line1;
    line1 << "Existem " << articulationPoints.size() << " aeroportos importantes (Pontos de articulação)." << endl;

    vector<string> text = {line1.str()};

    vector<string> options = {"Voltar"};

    Menu consultar_rede_global("Consultar rede global", "opção", options, text, false, true);
    consultar_rede_global.render();

    if(consultar_rede_global.optionIsSelected() && consultar_rede_global.getOption() == 0){
        consultar_rede();
    }
}

void menus::consultar_rede_companhia(){
    vector<string> options = {"Voltar"};

    vector<string> airlines;
    for(auto it = m.getAirlines().begin(); it != m.getAirlines().end(); it++) {
        airlines.push_back(it->second.getCode());
    }

    Menu airlines_menu("Consultar rede de uma companhia", "companhia", options, airlines, true, true);
    airlines_menu.render();

    if(airlines_menu.optionIsSelected() && airlines_menu.getOption() == 0){
        consultar_rede();
    }

    string airlineCode = airlines_menu.getInput();
    Airline airline = m.getAirlines().find(airlineCode)->second;
    unordered_map<string, Airport> network = m.getUndirectedAirlineNetwork(airlineCode);
    set<string> articulationPoints = m.getArticulationPoints(network, airline.getFlights()[0]->getSource());

    stringstream line1;
    line1 << "Esta companhia area tem " << articulationPoints.size() << " aeroportos importantes (Pontos de articulação)." << endl;

    vector<string> text = {line1.str()};

    vector<string> options2 = {"Voltar"};

    Menu airline_menu("Rede " + airline.getName(), "opção", options2, text, false, true);
    airline_menu.render();

    if(airline_menu.optionIsSelected() && airline_menu.getOption() == 0){
        consultar_rede_companhia();
    }
}
