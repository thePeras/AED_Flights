#include "menus.h"
#include <iostream>
#include "Managing.h"
#include "Menu.h"
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <map>

#include "Validate.h"

using namespace std;

Managing m;
vector<string> travel_source_airports;
vector<string> travel_target_airports;
int max_num_flights = 1;          //user input in results menu
set<string> considered_airlines;  //user input in results menu
list<list<Flight*>> possible_paths;
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
    for(auto elem : m.getAirlines()){
        considered_airlines.insert(elem.first);
    }
    mainMenu();
}

void menus::mainMenu(){
    /*
    vector<string> airports = {"OPO", "LIS", "FAO"};
    vector<string> targets = {"SYD", "MEB", "BNE", "PER"};
    set<string> airlines= {"TAP", "UAE"};
    for (auto el : m.possiblePaths(airports, targets, 3, airlines)) {
        for (auto el2 : el) {
            cout << el2->getSource() << " -> " << el2->getTarget() << " | " << el2->getAirline() << endl;
        }
        cout << endl;
    }
     */

    /*
    set<string> airlines = {"TAP", "EZY"};
    for (auto el : m.possiblePaths("OPO", "GVA", 2, airlines)) {
        for (auto el2 : el) {
            cout << el2->getSource() << " -> " << el2->getTarget() << " | " << el2->getAirline() << endl;
        }
        cout << endl;
    }
    */

    possible_paths.clear();

    vector<string> options = {
            "Sair",
            "Viajar",
            "Consultar Aeroporto",
            "Consultar Rede",
            "Os meus bilhetes"
    };

    Menu MainMenu("Inicio", "Escolha uma opção: ", options, {}, true);
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

    Menu digit_airport("Consultar aeroporto", "código do aeroporto: ", options, airports, true, true, 10);
    digit_airport.render();

    if(digit_airport.optionIsSelected() && digit_airport.getOption() == 0){
       mainMenu();
    }

    Airport theAirport = m.getAirports().find(digit_airport.getInput())->second;
    consultar_aeroporto(theAirport);
}

void menus::consultar_aeroporto(Airport& airport){

    vector<string> options = {
            "Voltar",
            "Informação sobre os voos",
            "Ver destinos diretos",
            "Ver destinos com X voos",
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

    Menu consult_airport(airportName, "Opção: ", options, curiosities, true, true, 1);
    consult_airport.render();

    if(consult_airport.optionIsSelected()){
        switch (consult_airport.getOption()) {
            case 0: mainMenu(); break;
            case 1: voos_aeroporto(airport); break;
            case 2: cout << "not yet implemented" << endl; break;
            case 3: {
                vector<string> options = { "Voltar" };

                Menu target_at_x("Destinos a X voos de" + airportName, "Digite um x: ", options, {}, true, true, 1, true);
                target_at_x.render();

                if(target_at_x.optionIsSelected() && target_at_x.getOption() == 0){
                    return consultar_aeroporto(airport);
                }

                int x = stoi(target_at_x.getInput());
                set<string> targets = m.reachableAirports(airport.getCode(), x);
                vector<string> targets_vector;
                vector<string> targets_codes;

                if(targets.size() > 0){
                    targets_vector.push_back(to_string(targets.size()) + " destinos alcançáveis a "+ to_string(x) + " voos.");
                    for(int i=0; i<3; i++) targets_vector.push_back("");
                }

                for(string targetCode : targets){
                    Airport target_airport = m.getAirports().find(targetCode)->second;
                    string new_target = targetCode + " - " + target_airport.getName() + ", " + target_airport.getCountry();
                    targets_vector.push_back(new_target);
                    targets_codes.push_back(targetCode);
                }

                Menu target_at_x_list("Destinos a " + to_string(x) + " voos de" + airportName, "Opção: ", options, targets_vector, targets_codes,true, true, 2, false);
                target_at_x_list.render();

                if(target_at_x_list.optionIsSelected() && target_at_x_list.getOption() == 0){
                    return consultar_aeroporto(airport);
                }

                Airport target = m.getAirports().find(target_at_x_list.getInput())->second;
                return consultar_aeroporto(target);
            }
        }
    }
}

void menus::voos_aeroporto(Airport airport){
    vector<string> options = {"Voltar"};
    map<string,pair<double,set<string>>> all_targets;
    vector<string> flights;

    for(auto &flight : airport.getFlights()){
        all_targets[flight->getTarget()].first = flight->getDistance();
        all_targets[flight->getTarget()].second.insert(flight->getAirline());
    }

    for(auto &target : all_targets){
        string targetString = target.first;
        flights.push_back(targetString);
        string distance = to_string((int) target.second.first) + " kms";
        flights.push_back(distance);
        string airlines = "";
        for(auto &airline : target.second.second){
            airlines += airline + " ";
        }
        flights.push_back(airlines);
    }

    flights.insert(flights.begin(), "Destino");
    flights.insert(flights.begin()+1, "Distância");
    flights.insert(flights.begin()+2, "Companhias");

    Menu airport_flights("Voos - Aeroporto", "Opção: ", options, flights, false, true, 3);
    airport_flights.render();

    if(airport_flights.optionIsSelected() && airport_flights.getOption() == 0){
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

    Menu travel_menu("Viajar - " + title, "Opção: ", options, {});
    travel_menu.render();

    if(travel_menu.optionIsSelected()){
        switch (travel_menu.getOption()) {

            case 0:
                travel_source_airports.clear();
                travel_target_airports.clear();
                mainMenu();
                break;

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

    Menu digit_airport("Viajar - Aeroporto", "digite o código do aeroporto: ", options, airports, true, true, 10);
    digit_airport.render();

    if(digit_airport.optionIsSelected() && digit_airport.getOption() == 0){
        string title = travel_source_airports.size() == 0 ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string airport = digit_airport.getInput();

    if(travel_source_airports.size() == 0) {
        travel_source_airports.push_back(airport);
        menu_viajar("Para onde?");
    }
    else {
        travel_target_airports.push_back(airport);
        menu_results();
    }
}

void menus::menu_pais(){
    vector<string> options = { "Voltar" };

    auto countryCities = m.getCountryCities();  //get countries
    vector<string> countries;
    for(auto & countryCitie : countryCities){
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    Menu country_menu("Viajar - País", "Introduza um país: ", options, countries, true, true);
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

    Menu choice_allOrOne_country("Viajar - " + country, "Escolha uma opção: ", options_cities, cities, true, true, 3);
    choice_allOrOne_country.render();

    if(choice_allOrOne_country.optionIsSelected()){
        switch (choice_allOrOne_country.getOption()) {
            case 0: menu_pais(); return;
            case 1: {
                vector<string> airports = m.getAirportsInCountry(country);
                //TODO: Run the algorithm
                return;
            }
        }
    }

    //selected a city
    string city = choice_allOrOne_country.getInput();
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

    Menu airport_menu("Viajar - Aeroporto", "Código do aeroporto: ", airport_options, airports, true, true, 10);
    airport_menu.render();

    if(airport_menu.optionIsSelected()){
        switch (airport_menu.getOption()) {
            case 0: menus_pais_cidades(country); return;
            case 1: {
                //TODO: run the algorithm with aiports vector
                return;
            }
        }
    }

    string airport = airport_menu.getInput();
    Airport a = m.getAirports().find(airport)->second;

    //TODO: run the algorithm with a
}

void menus::menu_coordenadas(){
    vector<string> options_viajar = {"Voltar"};

    Menu latitude_menu("Viajar - Coordenadas", "latitude: ", options_viajar);
    latitude_menu.render();

    if(latitude_menu.optionIsSelected() && latitude_menu.getOption() == 0){
        string title = travel_source_airports.size() == 0 ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string latitude = latitude_menu.getInput();
    do{
        latitude = latitude_menu.getInput();
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
            "Rede de uma companhia",
            "Rede de um país"
    };

    Menu consult_network("Consultar rede", "Opção: ", options, {}, true);
    consult_network.render();

    if(consult_network.optionIsSelected()){
        switch (consult_network.getOption()) {
            case 0: mainMenu(); break;
            case 1: consultar_rede_global(); break;
            case 2: digitar_companhia(); break;
            case 3: digitar_pais(); break;
        }
    }
}

void menus::consultar_rede_global(){
    unordered_map<string, Airport> network = m.getUndirectedGlobalNetwork();

    set<string> articulationPoints = m.getArticulationPoints(network);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    line1 << "Existem ao todo, " << m.getAirports().size() << " aeroportos.";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "A rede global de aeroportos tem um diâmetro aproximadamente de " << m.getDiameter(m.getAirports(), false) << " aeroportos.";
    line4 << "O diâmetro em kms é de aproximadamente " << m.getWeightedDiameter(m.getAirports(), false) << " kms.";
    int numberOfComponents = m.numberOfComponents(network);
    line5 << "A rede global tem " << numberOfComponents << " componentes conexos.";

    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str()};

    vector<string> options = {
            "Voltar",
            "Ver todo os aeroportos",
            "Ver somente os aeroportos importantes",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu global_network("Rede global", "Opção: ", options, text, true, true, 1);
    global_network.render();

    vector<string> back_options = {"Voltar"};

    if(global_network.optionIsSelected()){
        switch (global_network.getOption()) {
            case 0: consultar_rede(); break;
            case 1: {
                vector<string> airports;
                for(auto pair : m.getAirports()){
                    stringstream line;
                    Airport airport = pair.second;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - " << airport.getCountry();
                    airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos - Rede Global", "Opção: ", back_options, airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_global();
                break;
            }
            case 2: {
                vector<string> important_airports;
                for(string airportCode : articulationPoints){
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - " << airport.getCountry();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - Rede global", "Opção: ", back_options, important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_global();
                break;
            }
            case 3: {
                string menu_text = "O diametro preciso é de " + to_string(m.getDiameter(m.getAirports(), true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede Global", "Opção: ", back_options, {menu_text}, false, true, 1);
                precise_diameter.render();
                consultar_rede_global();
                break;
            }
            case 4: {
                string menu_text = "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(m.getAirports(), true)) + " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede Global", "Opção: ", back_options, {menu_text}, false, true, 1);
                precise_diameter.render();
                consultar_rede_global();
                break;
            };
        }
    }
}

void menus::digitar_companhia() {
    vector<string> options = {"Voltar"};

    vector<string> airlines;
    for(auto it = m.getAirlines().begin(); it != m.getAirlines().end(); it++) {
        airlines.push_back(it->second.getCode());
    }

    Menu airlines_menu("Consultar rede de uma companhia", "Companhia aerea: ", options, airlines, true, true, 10);
    airlines_menu.render();

    if(airlines_menu.optionIsSelected() && airlines_menu.getOption() == 0){
        consultar_rede();
    }
    string airlineCode = airlines_menu.getInput();
    consultar_rede_companhia(airlineCode);
}

void menus::consultar_rede_companhia(string airlineCode){

    Airline airline = m.getAirlines().find(airlineCode)->second;
    unordered_map<string, Airport> directedNetwork = m.getAirlineNetwork(airlineCode, true);
    unordered_map<string, Airport> undirectedNetwork = m.getAirlineNetwork(airlineCode, false);
    set<string> articulationPoints = m.getArticulationPoints(undirectedNetwork);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    line1 << "A rede desta companhia area tem " << airline.getAirports().size() << " aeroportos.";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "O diâmetro da rede é de aproximadamente " << m.getDiameter(directedNetwork, false) << " aeroportos.";
    line4 << "Em kms, o diâmetro é de aproximadamente " << m.getWeightedDiameter(directedNetwork, false) << " kms.";
    int numberOfComponents = m.numberOfComponents(directedNetwork);
    line5 << "A companhia tem " << numberOfComponents << " componente";
    if(numberOfComponents > 1) line5 << "s.";


    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str()};

    vector<string> options2 = {
            "Voltar",
            "Ver todos os aeroportos",
            "Ver somente aeroportos importantes",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu airline_menu("Rede " + airline.getName(), "Opção: ", options2, text, false, true, 1);
    airline_menu.render();

    vector<string> voltar_options = {"Voltar"};

    if(airline_menu.optionIsSelected()){
        switch (airline_menu.getOption()) {
            case 0: consultar_rede(); break;
            case 1: {
                vector<string> airports;
                for(string airportCode : airline.getAirports()){
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - " << airport.getCountry();
                    airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos - " + airlineCode, "Opção: ", voltar_options, airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 2: {
                vector<string> important_airports;
                for(string airportCode : articulationPoints){
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - " << airport.getCountry();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - " + airlineCode, "Opção: ", voltar_options, important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 3: {
                string menutext = "O diametro preciso é de " + to_string(m.getDiameter(directedNetwork, true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede " + airline.getName(), "Opção: ", voltar_options, {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 4: {
                string menutext = "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(directedNetwork, true)) + " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede " + airline.getName(), "Opção: ", voltar_options, {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_companhia(airlineCode);
                break;
            };
        }
    }
}

void menus::digitar_pais() {
    vector<string> options = { "Voltar" };

    auto countryCities = m.getCountryCities();
    vector<string> countries;
    for(auto & countryCity : countryCities){
        countries.push_back(countryCity.first);
    }
    sort(countries.begin(), countries.end());


    Menu country_menu("Consultar Rede - País", "Introduza um país: ", options, countries, true, true);
    country_menu.render();

    if(country_menu.optionIsSelected() && country_menu.getOption() == 0){
        consultar_rede();
        return;
    }

    string country = country_menu.getInput();
    consultar_rede_pais(country);
}

void menus::consultar_rede_pais(string country){

    unordered_map<string, Airport> directedNetwork = m.getCountryNetwork(country, true);
    unordered_map<string, Airport> undirectedNetwork = m.getCountryNetwork(country, false);
    set<string> articulationPoints = m.getArticulationPoints(undirectedNetwork);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    line1 << "Existem " << directedNetwork.size() << " aeroportos neste país.";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "O diâmetro da rede é de aproximadamente " << m.getDiameter(directedNetwork, false) << " aeroportos.";
    line4 << "Em kms, o diâmetro é de aproximadamente " << m.getWeightedDiameter(directedNetwork, false) << " kms.";
    int numberOfComponents = m.numberOfComponents(directedNetwork);
    line5 << "A rede deste país tem " << numberOfComponents << " componente";
    if(numberOfComponents > 1) line5 << "s.";


    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str()};

    vector<string> options2 = {
            "Voltar",
            "Ver todos os aeroportos",
            "Ver somente aeroportos importantes",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu country_network_menu("Rede " + country, "Opção: ", options2, text, false, true, 1);
    country_network_menu.render();

    vector<string> voltar_options = {"Voltar"};

    if(country_network_menu.optionIsSelected()){
        switch (country_network_menu.getOption()) {
            case 0: consultar_rede(); break;
            case 1: {
                vector<string> airports;
                for(auto & airport : directedNetwork){
                    stringstream line;
                    line << airport.second.getCode() << " - " << airport.second.getName() << " - " << airport.second.getCity();
                    airports.push_back(line.str());
                }
                Menu all_airports_menu("Aeroportos - " + country, "Opção: ", voltar_options, airports, false, true, 1);
                all_airports_menu.render();
                consultar_rede_pais(country);
                break;
            }
            case 2: {
                vector<string> important_airports;
                for(string airportCode : articulationPoints){
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - " + country, "Opção: ", voltar_options, important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_pais(country);
                break;
            }
            case 3: {
                string menutext = "O diametro preciso é de " + to_string(m.getDiameter(directedNetwork, true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede " + country, "Opção: ", voltar_options, {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_pais(country);
                break;
            }
            case 4: {
                string menutext = "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(directedNetwork, true)) + " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede " + country, "Opção: ", voltar_options, {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_pais(country);
                break;
            };
        }
    }
}

void menus::menu_results() {
    vector<string> options = {"Voltar", "Filtrar"};

    possible_paths = m.possiblePaths(travel_source_airports, travel_target_airports,max_num_flights, considered_airlines);

    vector<string> results;
    for(auto trip : possible_paths){
        for(auto flight : trip){
            results.push_back(flight->getSource() + " -> " + flight->getTarget() + " : " + flight->getAirline());
        }
    }

    Menu menu_results("Resultados", "Escolha uma opção ou o ID de um voo: ", options, results, true, true, 10);
    menu_results.render();

    if(menu_results.optionIsSelected() && menu_results.getOption() == 0){
        travel_source_airports.clear();
        travel_target_airports.clear();
        menu_viajar("De onde?");
    }
    else if(menu_results.optionIsSelected() && menu_results.getOption() == 1){
        menu_filtrar();
    }

    string id = menu_results.getInput();
    cout << "Voo escolhido: " << id << endl;
}


void menus::menu_filtrar() {
    vector<string> options = {"Voltar", "Companhias", "Escalas"};
    Menu menu_filtrar("Filtrar", "Escolha uma opção: ", options, {});
    menu_filtrar.render();

    if(menu_filtrar.optionIsSelected() && menu_filtrar.getOption() == 0){
        menu_results();
    }
    else if(menu_filtrar.optionIsSelected() && menu_filtrar.getOption() == 1){
        menu_companhias();
    }
    else if(menu_filtrar.optionIsSelected() && menu_filtrar.getOption() == 2){
        menu_escala();
    }


}

void menus::menu_companhias() {
    set<string> airlines;
    for(auto trip : possible_paths){
        for(auto flight : trip){
            airlines.insert(flight->getAirline());
        }
    }

    vector<string> options = {"Voltar"};
    vector<string> results;
    for(auto airline : airlines){
        results.push_back(airline);
    }

    Menu menu_companhias("Companhias", "Escolha uma opção: ", options, results, true, true, 10);
    menu_companhias.render();

    if(menu_companhias.optionIsSelected() && menu_companhias.getOption() == 0){
        menu_filtrar();
    }

    string airline = menu_companhias.getInput();
    considered_airlines.clear();
    considered_airlines.insert(airline);
    menu_results();
}

void menus::menu_escala() {
    vector<string> options = {"Voltar"};

    vector<string> results;
    for(int i = 1; i <= 10; i++){
        results.push_back(to_string(i));
    }

    Menu menu_escala("Escala", "Escolha o número de voos: ", options, results, true, true);
    menu_escala.render();

    if(menu_escala.optionIsSelected() && menu_escala.getOption() == 0){
        menu_filtrar();
    }

    auto num = menu_escala.getInput();
    max_num_flights = stoi(num);
    menu_results();
}

