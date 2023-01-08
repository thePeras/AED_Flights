#include "menus.h"
#include <iostream>
#include "Managing.h"
#include "Menu.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "Validate.h"

using namespace std;

Managing m;
vector<string> travel_source_airports;
vector<string> travel_target_airports;

void menus::exit_action() {
    cout << endl << "Leaving..." << endl;
    cout << "Goodbye!" << endl;
    exit(0);
}

// MENUS

// MAIN MENU

menus::menus() {
    m.readFiles();
    mainMenu();
}

void menus::mainMenu() {
    travel_source_airports.clear();
    travel_target_airports.clear();

    vector<string> options = {
            "Sair",
            "Viajar",
            "Consultar Aeroporto",
            "Consultar Rede"
    };

    Menu MainMenu("Inicio", "Escolha uma opção: ", options, {}, true);

    MainMenu.render();


    if (MainMenu.optionIsSelected()) {
        switch (MainMenu.getOption()) {
            case 0:
                exit_action();
                break;
            case 1:
                menu_viajar("De onde?");
                break;
            case 2:
                digitar_aeroporto();
                break;
            case 3:
                consultar_rede();
                break;
        }
    }
}

// CONSULTAR AEROPORTO MENU

void menus::digitar_aeroporto() {
    vector<string> options = {"Voltar"};

    vector<string> airports;
    for (auto it = m.getAirports().begin(); it != m.getAirports().end(); it++) {
        airports.push_back(it->second.getCode());
    }

    sort(airports.begin(), airports.end());

    Menu digit_airport("Consultar aeroporto", "código do aeroporto: ", options, airports, true, true, 10);
    digit_airport.render();

    if (digit_airport.optionIsSelected() && digit_airport.getOption() == 0) {
        mainMenu();
    }

    Airport theAirport = m.getAirports().find(digit_airport.getInput())->second;
    consultar_aeroporto(theAirport);
}

void menus::consultar_aeroporto(Airport &airport) {

    vector<string> options = {
            "Voltar",
            "Informação sobre os voos",
            "Ver destinos diretos",
            "Ver destinos com X voos",
    };

    stringstream curiosity1;
    curiosity1 << "Neste aeroporto descola";
    if (airport.getFlights().size() != 1) curiosity1 << "m";
    if (airport.getFlights().size() < 10) curiosity1 << " apenas";
    curiosity1 << " " << airport.getFlights().size() << " voo";
    if (airport.getFlights().size() != 1) curiosity1 << "s";

    stringstream curiosity2;
    curiosity2 << "Operam " << airport.getAirlines().size() << " companhias areas";

    stringstream curiosity3;
    string mostDistanceCountry = m.mostDistantCountry(airport.getCode(), 2);
    curiosity3 << "Com 2 voos consegues ir ao país " << mostDistanceCountry;

    vector<string> curiosities = {curiosity1.str(), curiosity2.str(), curiosity3.str()};

    string airportName =
            " (" + airport.getCode() + ") Aeroporto - " + airport.getName() + ", " + airport.getCountry() + " | " +
            to_string(airport.getLocation().getLatitude()) + " " + to_string(airport.getLocation().getLongitude());

    Menu consult_airport(airportName, "Opção: ", options, curiosities, true, true, 1);
    consult_airport.render();

    vector<string> back_options = {"Voltar"};

    if (consult_airport.optionIsSelected()) {
        switch (consult_airport.getOption()) {
            case 0:
                mainMenu();
                break;
            case 1:
                voos_aeroporto(airport);
                break;
            case 2: {
                set<string> targets = m.reachableAirports(airport.getCode(), 1);
                vector<string> targets_vector;
                vector<string> targets_codes;

                if (targets.size() > 0) {
                    targets_vector.push_back(to_string(targets.size()) + " destinos diretos diferentes.");
                    for (int i = 0; i < 3; i++) targets_vector.push_back("");
                }

                for (string targetCode: targets) {
                    Airport target_airport = m.getAirports().find(targetCode)->second;
                    string new_target =
                            targetCode + " - " + target_airport.getName() + ", " + target_airport.getCountry();
                    targets_vector.push_back(new_target);
                    targets_codes.push_back(targetCode);
                }

                Menu direct_targets("Destinos diretos" + airportName, "Opção: ", back_options, targets_vector,
                                    targets_codes, true, true, 2, false);
                direct_targets.render();

                if (direct_targets.optionIsSelected() && direct_targets.getOption() == 0) {
                    return consultar_aeroporto(airport);
                }

                Airport target = m.getAirports().find(direct_targets.getInput())->second;
                return consultar_aeroporto(target);
            }
            case 3: {
                Menu target_at_x("Destinos a X voos de" + airportName, "Digite um x: ", back_options, {}, true, true, 1,
                                 true);
                target_at_x.render();

                if (target_at_x.optionIsSelected() && target_at_x.getOption() == 0) {
                    return consultar_aeroporto(airport);
                }

                int x = stoi(target_at_x.getInput());
                set<string> targets = m.reachableAirports(airport.getCode(), x);
                vector<string> targets_vector;
                vector<string> targets_codes;

                if (targets.size() > 0) {
                    targets_vector.push_back(
                            to_string(targets.size()) + " destinos alcançáveis a " + to_string(x) + " voos.");
                    for (int i = 0; i < 3; i++) targets_vector.push_back("");
                }

                for (string targetCode: targets) {
                    Airport target_airport = m.getAirports().find(targetCode)->second;
                    string new_target =
                            targetCode + " - " + target_airport.getName() + ", " + target_airport.getCountry();
                    targets_vector.push_back(new_target);
                    targets_codes.push_back(targetCode);
                }

                Menu target_at_x_list("Destinos a " + to_string(x) + " voos de" + airportName, "Opção: ", back_options,
                                      targets_vector, targets_codes, true, true, 2, false);
                target_at_x_list.render();

                if (target_at_x_list.optionIsSelected() && target_at_x_list.getOption() == 0) {
                    return consultar_aeroporto(airport);
                }

                Airport target = m.getAirports().find(target_at_x_list.getInput())->second;
                return consultar_aeroporto(target);
            }
        }
    }
}

void menus::voos_aeroporto(Airport airport) {
    vector<string> options = {"Voltar"};
    map<string, pair<double, vector<string>>> all_targets;
    vector<string> flights;

    for (auto &flight: airport.getFlights()) {
        all_targets[flight->getTarget()].first = flight->getDistance();
        all_targets[flight->getTarget()].second = flight->getAirlines();
    }

    vector<string> targets_codes;
    for (auto &target: all_targets) {
        string targetString = target.first;
        targets_codes.push_back(targetString);
        flights.push_back(targetString);
        string distance = to_string((int) target.second.first) + " kms";
        flights.push_back(distance);
        string airlines = "";
        for (auto &airline: target.second.second) {
            airlines += airline + " ";
        }
        flights.push_back(airlines);
    }

    flights.insert(flights.begin(), "Destino");
    flights.insert(flights.begin() + 1, "Distância");
    flights.insert(flights.begin() + 2, "Companhias");

    Menu airport_flights("Voos - Aeroporto", "Escolha uma opção ou o código de um aeroporto: ", options, flights,
                         targets_codes, true, true, 3);
    airport_flights.render();

    if (airport_flights.optionIsSelected() && airport_flights.getOption() == 0) {
        consultar_aeroporto(airport);
    }

    Airport theAirport = m.getAirports().find(airport_flights.getInput())->second;
    consultar_aeroporto(theAirport);
}

// VIAJAR MENU

void menus::menu_viajar(string title) {
    vector<string> options = {
            "Voltar",
            "Aeroporto (TAG)",
            "País",
            "Coordenadas"
    };

    Menu travel_menu("Viajar - " + title, "Opção: ", options, {});
    travel_menu.render();

    if (travel_menu.optionIsSelected()) {
        switch (travel_menu.getOption()) {

            case 0:
                travel_source_airports.clear();
                travel_target_airports.clear();
                mainMenu();
                break;

            case 1:
                aeroporto_input();
                break;
            case 2:
                menu_pais();
                break;
            case 3:
                menu_coordenadas();
                break;
        }
    }
}

void menus::aeroporto_input() {
    vector<string> options = {"Voltar"};

    vector<string> airports;
    for (auto airport: m.getAirports()) {
        airports.push_back(airport.second.getCode());
    }
    sort(airports.begin(), airports.end());

    Menu digit_airport("Viajar - Aeroporto", "Digite o código do aeroporto: ", options, airports, true, true, 10);
    digit_airport.render();

    if (digit_airport.optionIsSelected() && digit_airport.getOption() == 0) {
        string title = travel_source_airports.empty() ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string airport = digit_airport.getInput();

    if (travel_source_airports.empty()) {
        travel_source_airports.push_back(airport);
        menu_viajar("Para onde?");
    } else {
        travel_target_airports.push_back(airport);
        escolher_rede();
    }
}

void menus::menu_pais() {
    vector<string> options = {"Voltar"};

    auto countryCities = m.getCountryCities();  //get countries
    vector<string> countries;
    for (auto &countryCitie: countryCities) {
        countries.push_back(countryCitie.first);
    }
    sort(countries.begin(), countries.end());


    Menu country_menu("Viajar - País", "Introduza um país: ", options, countries, true, true);
    country_menu.render();

    if (country_menu.optionIsSelected() && country_menu.getOption() == 0) {
        string title = travel_source_airports.empty() ? "De onde?" : "Para onde?";
        menu_viajar(title);
        return;
    }

    string country = country_menu.getInput();
    menus_pais_cidades(country);
}

void menus::menus_pais_cidades(string country) {
    vector<string> cities = m.getCountryCities().find(country)->second;

    vector<string> options_cities = {
            "Voltar",
            "Selecionar todas as cidades"
    };

    sort(cities.begin(), cities.end());
    Menu choice_allOrOne_country("Viajar - " + country, "Escolha uma opção ou uma cidade: ", options_cities, cities,
                                 true, true, 3);
    choice_allOrOne_country.render();

    if (choice_allOrOne_country.optionIsSelected()) {
        switch (choice_allOrOne_country.getOption()) {
            case 0:
                menu_pais();
                return;
            case 1: {
                if (travel_source_airports.empty()) {
                    string title = "Para onde?";
                    travel_source_airports = m.getAirportsInCountry(country);
                    menu_viajar(title);
                    return;
                } else {
                    travel_target_airports = m.getAirportsInCountry(country);
                    escolher_rede();
                    return;
                }
            }
        }
    }

    //selected a city
    string city = choice_allOrOne_country.getInput();
    menu_cidade(city, country);
}

void menus::menu_cidade(string city, string country) {
    vector<string> airports = m.getAirportsInCity(city, country);

    if (airports.size() == 1) {
        Airport airport = m.getAirports().find(airports[0])->second;
        if (travel_source_airports.empty()) {
            travel_source_airports.push_back(airport.getCode());
            menu_viajar("Para onde?");
            return;
        } else {
            travel_target_airports.push_back(airport.getCode());
            escolher_rede();
            return;
        }
    }

    vector<string> airport_options = {
            "Voltar",
            "Selecionar todos os aeroportos",
    };

    Menu airport_menu("Viajar - Aeroporto", "Código do aeroporto: ", airport_options, airports, true, true, 10);
    airport_menu.render();

    if (airport_menu.optionIsSelected()) {
        switch (airport_menu.getOption()) {
            case 0:
                menus_pais_cidades(country);
                return;
            case 1: {
                if (travel_source_airports.empty()) {
                    string title = "Para onde?";
                    travel_source_airports = airports;
                    menu_viajar(title);
                    return;
                } else {
                    travel_target_airports = airports;
                    escolher_rede();
                    return;
                }
            }
        }
    }

    string airport = airport_menu.getInput();
    Airport a = m.getAirports().find(airport)->second;
    if (travel_source_airports.empty()) {
        travel_source_airports.push_back(a.getCode());
        menu_viajar("Para onde?");
        return;
    } else {
        travel_target_airports.push_back(a.getCode());
        escolher_rede();
        return;
    }
}

void menus::menu_coordenadas() {
    vector<string> options_viajar = {"Voltar"};

    Menu latitude_menu("Viajar - Coordenadas", "Latitude: ", options_viajar);
    latitude_menu.render();

    if (latitude_menu.optionIsSelected() && latitude_menu.getOption() == 0) {
        string title = travel_source_airports.empty() ? "De onde?" : "Para onde?";
        menu_viajar(title);
    }

    string latitude = latitude_menu.getInput();
    do {
        latitude = latitude_menu.getInput();
    } while (!Validate::latitude(latitude));

    string longitude;
    do {
        cout << "Longitude: ";
        getline(cin, longitude);
    } while (!Validate::longitude(longitude));

    string radius;
    do {
        cout << "Raio: ";
        getline(cin, radius);
    } while (!Validate::radius(radius));

    vector<string> airport_options = {
            "Voltar",
            "Selecionar todos os aeroportos",
    };

    vector<string> airports = m.getAirportsInRadius(Location(stod(latitude), stod(longitude)), stod(radius));

    if (airports.empty()) {
        cout << endl << "Não foram encontrados aeroportos no raio especificado." << endl;
        menu_coordenadas();
        return;
    }

    Menu airport_menu("Viajar - Aeroporto", "Código do aeroporto: ", airport_options, airports, true, true, 10);
    airport_menu.render();

    if (airport_menu.optionIsSelected()) {
        switch (airport_menu.getOption()) {
            case 0: {
                string title = travel_source_airports.empty() ? "De onde?" : "Para onde?";
                menu_viajar(title);
                return;
            }
            case 1: {
                if (travel_source_airports.empty()) {
                    string title = "Para onde?";
                    travel_source_airports = airports;
                    menu_viajar(title);
                    return;
                } else {
                    travel_target_airports = airports;
                    menu_results(m.getAirports());
                    return;
                }
            }
        }
    }

    string airport = airport_menu.getInput();
    Airport a = m.getAirports().find(airport)->second;
    if (travel_source_airports.empty()) {
        travel_source_airports.push_back(a.getCode());
        menu_viajar("Para onde?");
        return;
    } else {
        travel_target_airports.push_back(a.getCode());
        menu_results(m.getAirports());
        return;
    }
}

void menus::escolher_rede() {
    vector<string> options = {
            "Voltar",
            "Rede Global",
            "Rede de companhia(s)",
    };

    Menu travel_in_network("Viajar em que rede?", "Opção: ", options, {}, true);
    travel_in_network.render();

    if (travel_in_network.optionIsSelected()) {
        switch (travel_in_network.getOption()) {
            case 0:
                mainMenu();
                break;
            case 1: {
                menu_results(m.getAirports());
                break;
            };
            case 2: {
                options = {"Voltar", "Pesquisar"};

                vector<string> airlines;
                vector<string> airlines_codes;
                airlines_codes.push_back("Companhias selecionadas: []");
                airlines_codes.push_back("");
                airlines_codes.push_back("");
                for (auto it = m.getAirlines().begin(); it != m.getAirlines().end(); it++) {
                    airlines.push_back(it->second.getCode());
                    airlines_codes.push_back(it->second.getCode() + " - " + it->second.getName());
                }
                sort(airlines_codes.begin() + 3, airlines_codes.end(), [](const string &a, const string &b) {
                    if (isdigit(a[0]) && isdigit(b[0])) return a < b;
                    if (isdigit(a[0])) return false;
                    if (isdigit(b[0])) return true;
                    return a < b;
                });

                set<string> selected_airlines;
                Menu airlines_menu("Viajar - Rede Companhias", "Código da companhia: ", options, airlines_codes,
                                   airlines, true, true, 3);
                airlines_menu.render();
                while (!airlines_menu.optionIsSelected()) {
                    string digited_airline = airlines_menu.getInput();
                    cout << digited_airline << endl;
                    selected_airlines.insert(digited_airline);

                    string selected_airlines_string = "Companhias selecionadas: [";
                    for (string airline: selected_airlines) {
                        selected_airlines_string += airline + ", ";
                    }
                    selected_airlines_string += "]";

                    airlines_codes[0] = selected_airlines_string;
                    airlines_menu.setList(airlines_codes);

                    airlines_menu.render();
                }

                unordered_map<string, Airport> airlines_network = m.getAirlinesNetwork(selected_airlines, false);
                menu_results(airlines_network);

                break;
            }
        }
    }
}


// CONSULTAR REDE MENU

void menus::consultar_rede() {
    vector<string> options = {
            "Voltar",
            "Rede Global",
            "Rede de uma companhia",
            "Rede de um país"
    };

    Menu consult_network("Consultar rede", "Opção: ", options, {}, true);
    consult_network.render();

    if (consult_network.optionIsSelected()) {
        switch (consult_network.getOption()) {
            case 0:
                mainMenu();
                break;
            case 1:
                consultar_rede_global();
                break;
            case 2:
                digitar_companhia();
                break;
            case 3:
                digitar_pais();
                break;
        }
    }
}

void menus::consultar_rede_global() {
    unordered_map<string, Airport> network = m.getUndirectedGlobalNetwork();
    unordered_map<string, Airport> directedNetwork = m.getAirports();
    set<string> articulationPoints = m.getArticulationPoints(network);
    vector<vector<string>> stronglyConnectedComponents = m.getStronglyConnectedComponentes(directedNetwork);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    stringstream line6;
    stringstream line7;
    line1 << "Existem ao todo, " << m.getAirports().size() << " aeroportos.";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "A rede global de aeroportos tem um diâmetro aproximadamente de " << m.getDiameter(m.getAirports(), false)
          << " aeroportos.";
    line4 << "O diâmetro em kms é de aproximadamente " << m.getWeightedDiameter(m.getAirports(), false) << " kms.";
    int numberOfComponents = m.numberOfComponents(network);
    line5 << "A rede global tem " << numberOfComponents << " componentes conexos.";
    line7 << "O conjunto de aeroportos fortemente conexos é de " << stronglyConnectedComponents.size()
          << " (Componentes fortemente conexos).";
    line6 << "Os aeroportos com mais voos diferentes são: ";
    vector<pair<string, int>> topAirports = m.getTopAirports(4, directedNetwork);
    for (pair<string, int> airport: topAirports) {
        line6 << endl << "\t\t-" << directedNetwork.find(airport.first)->second.getName() << ", "
              << directedNetwork.find(airport.first)->second.getCity() << " (" << airport.second << " voos diferentes)";
    }


    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str(), line7.str(), line6.str()};

    vector<string> options = {
            "Voltar",
            "Ver todo os aeroportos",
            "Ver somente os aeroportos importantes",
            "Ver conjuntos de aeroportos fortemente conexos",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu global_network("Rede global", "Opção: ", options, text, true, true, 1);
    global_network.render();

    vector<string> back_options = {"Voltar"};

    if (global_network.optionIsSelected()) {
        switch (global_network.getOption()) {
            case 0:
                consultar_rede();
                break;
            case 1: {
                vector<string> airports;
                for (auto pair: m.getAirports()) {
                    stringstream line;
                    Airport airport = pair.second;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - "
                         << airport.getCountry();
                    airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos - Rede Global", "Opção: ", back_options, airports, false, true,
                                             1);
                important_airports_menu.render();
                consultar_rede_global();
                break;
            }
            case 2: {
                vector<string> important_airports;
                for (string airportCode: articulationPoints) {
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - "
                         << airport.getCountry();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - Rede global", "Opção: ", back_options,
                                             important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_global();
                break;
            }
            case 3: {
                vector<string> results;
                int max = 0;
                int count = 0;
                sort(stronglyConnectedComponents.begin(), stronglyConnectedComponents.end(),
                     [](const vector<string> &a, const vector<string> &b) {
                         return a.size() > b.size();
                     });
                for (auto component: stronglyConnectedComponents) {
                    string codes = "";
                    count = 0;
                    for (auto airportCode: component) {
                        Airport airport = m.getAirports().find(airportCode)->second;
                        count++;
                        codes += airport.getCode() + " <-> ";

                    }
                    if (count > max) max = count;
                    results.push_back(codes.erase(codes.length() - 5));
                }

                results.insert(results.begin(),
                               "O maior conjunto de aeroportos fortemente conexos tem " + to_string(max) +
                               " aeroportos.\n");

                Menu stronglyConnectedAirportsMenu("Aeroportos fortemente conexos - Rede global", "Opção: ",
                                                   back_options, results, false, true, 1);
                stronglyConnectedAirportsMenu.render();
                consultar_rede_global();
                break;
            }
            case 4: {
                string menu_text =
                        "O diametro preciso é de " + to_string(m.getDiameter(m.getAirports(), true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede Global", "Opção: ", back_options, {menu_text}, false,
                                      true, 1);
                precise_diameter.render();
                consultar_rede_global();
                break;
            }
            case 5: {
                string menu_text =
                        "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(m.getAirports(), true)) +
                        " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede Global", "Opção: ", back_options, {menu_text}, false,
                                      true, 1);
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
    for (auto it = m.getAirlines().begin(); it != m.getAirlines().end(); it++) {
        airlines.push_back(it->second.getCode());
    }

    Menu airlines_menu("Consultar rede de uma companhia", "Companhia aerea: ", options, airlines, true, true, 10);
    airlines_menu.render();

    if (airlines_menu.optionIsSelected() && airlines_menu.getOption() == 0) {
        consultar_rede();
    }
    string airlineCode = airlines_menu.getInput();
    consultar_rede_companhia(airlineCode);
}

void menus::consultar_rede_companhia(string airlineCode) {

    Airline airline = m.getAirlines().find(airlineCode)->second;
    unordered_map<string, Airport> directedNetwork = m.getAirlineNetwork(airlineCode, true);
    unordered_map<string, Airport> undirectedNetwork = m.getAirlineNetwork(airlineCode, false);
    set<string> articulationPoints = m.getArticulationPoints(undirectedNetwork);
    vector<vector<string>> stronglyConnectedComponents = m.getStronglyConnectedComponentes(directedNetwork);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    stringstream line6;
    stringstream line7;
    line1 << "A rede desta companhia area tem " << airline.getAirports().size() << " aeroportos.";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "O diâmetro da rede é de aproximadamente " << m.getDiameter(directedNetwork, false) << " aeroportos.";
    line4 << "Em kms, o diâmetro é de aproximadamente " << m.getWeightedDiameter(directedNetwork, false) << " kms.";
    int numberOfComponents = m.numberOfComponents(directedNetwork);
    line5 << "A companhia tem " << numberOfComponents << " componente";
    if (numberOfComponents > 1) line5 << "s."; else line5 << ".";
    line7 << "O conjunto de aeroportos fortemente conexos é de " << stronglyConnectedComponents.size()
          << " (Componentes fortemente conexos).";
    line6 << "Os aeroportos com mais voos diferentes são: ";


    vector<pair<string, int>> topAirports = m.getTopAirports(4, directedNetwork);
    for (pair<string, int> airport: topAirports) {
        line6 << endl << "\t\t-" << directedNetwork.find(airport.first)->second.getName() << ", "
              << directedNetwork.find(airport.first)->second.getCity() << " (" << airport.second << " voos diferentes)";
    }

    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str(), line7.str(), line6.str()};

    vector<string> options2 = {
            "Voltar",
            "Ver todos os aeroportos",
            "Ver somente aeroportos importantes",
            "Ver conjuntos de aeroportos fortemente conexos",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu airline_menu("Rede " + airline.getName(), "Opção: ", options2, text, false, true, 1);
    airline_menu.render();

    vector<string> voltar_options = {"Voltar"};

    if (airline_menu.optionIsSelected()) {
        switch (airline_menu.getOption()) {
            case 0:
                consultar_rede();
                break;
            case 1: {
                vector<string> airports;
                for (string airportCode: airline.getAirports()) {
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - "
                         << airport.getCountry();
                    airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos - " + airlineCode, "Opção: ", voltar_options, airports, false,
                                             true, 1);
                important_airports_menu.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 2: {
                vector<string> important_airports;
                for (string airportCode: articulationPoints) {
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity() << " - "
                         << airport.getCountry();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - " + airlineCode, "Opção: ", voltar_options,
                                             important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 3: {
                vector<string> results;
                int max = 0;
                int count = 0;
                sort(stronglyConnectedComponents.begin(), stronglyConnectedComponents.end(),
                     [](const vector<string> &a, const vector<string> &b) {
                         return a.size() > b.size();
                     });
                for (auto component: stronglyConnectedComponents) {
                    string codes = "";
                    count = 0;
                    for (auto airportCode: component) {
                        Airport airport = m.getAirports().find(airportCode)->second;
                        count++;
                        codes += airport.getCode() + " <-> ";

                    }
                    if (count > max) max = count;
                    results.push_back(codes.erase(codes.length() - 5));
                }

                results.insert(results.begin(),
                               "O maior conjunto de aeroportos fortemente conexos tem " + to_string(max) +
                               " aeroportos.\n");

                Menu components_menu("Componentes fortemente conexos - " + airlineCode, "Opção: ", voltar_options,
                                     results, false, true, 1);
                components_menu.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 4: {
                string menutext =
                        "O diametro preciso é de " + to_string(m.getDiameter(directedNetwork, true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede " + airline.getName(), "Opção: ", voltar_options,
                                      {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_companhia(airlineCode);
                break;
            }
            case 5: {
                string menutext =
                        "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(directedNetwork, true)) +
                        " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede " + airline.getName(), "Opção: ", voltar_options,
                                      {menutext}, false, true, 1);
                precise_diameter.render();
                consultar_rede_companhia(airlineCode);
                break;
            };
        }
    }
}

void menus::digitar_pais() {
    vector<string> options = {"Voltar"};

    auto countryCities = m.getCountryCities();
    vector<string> countries;
    for (auto &countryCity: countryCities) {
        countries.push_back(countryCity.first);
    }
    sort(countries.begin(), countries.end());


    Menu country_menu("Consultar Rede - País", "Introduza um país: ", options, countries, true, true);
    country_menu.render();

    if (country_menu.optionIsSelected() && country_menu.getOption() == 0) {
        consultar_rede();
        return;
    }

    string country = country_menu.getInput();
    consultar_rede_pais(country);
}

void menus::consultar_rede_pais(string country) {

    unordered_map<string, Airport> directedNetwork = m.getCountryNetwork(country, true);
    unordered_map<string, Airport> undirectedNetwork = m.getCountryNetwork(country, false);
    set<string> articulationPoints = m.getArticulationPoints(undirectedNetwork);
    vector<pair<string, int>> topAirports = m.getTopAirports(4, directedNetwork);
    vector<vector<string>> stronglyConnectedComponents = m.getStronglyConnectedComponentes(directedNetwork);

    stringstream line1;
    stringstream line2;
    stringstream line3;
    stringstream line4;
    stringstream line5;
    stringstream line6;
    stringstream line7;
    line1 << "Existem " << directedNetwork.size() << " aeroportos, ";
    line2 << "Dos quais " << articulationPoints.size() << " são importantes (Pontos de articulação).";
    line3 << "O diâmetro da rede é de aproximadamente " << m.getDiameter(directedNetwork, false) << " aeroportos.";
    line4 << "Em kms, o diâmetro é de aproximadamente " << m.getWeightedDiameter(directedNetwork, false) << " kms.";
    int numberOfComponents = m.numberOfComponents(directedNetwork);
    line5 << "A rede deste país tem " << numberOfComponents << " componente";
    if (numberOfComponents > 1) line5 << "s."; else line5 << ".";
    line6 << "Os aeroportos com mais voos diferentes são: ";
    line7 << "O conjunto de aeroportos fortemente conexos é de " << stronglyConnectedComponents.size()
          << " (Componentes fortemente conexos).";

    for (pair<string, int> airport: topAirports) {
        line6 << endl << "\t\t-" << directedNetwork.find(airport.first)->second.getName() << ", "
              << directedNetwork.find(airport.first)->second.getCity() << " (" << airport.second << " voos diferentes)";
    }


    vector<string> text = {line1.str(), line2.str(), line3.str(), line4.str(), line5.str(), line7.str(), line6.str()};

    vector<string> options2 = {
            "Voltar",
            "Ver todos os aeroportos",
            "Ver somente aeroportos importantes",
            "Ver conjuntos de aeroportos fortemente conexos",
            "Consultar diâmetro preciso de aeroportos",
            "Consultar diâmetro preciso em kms"
    };

    Menu country_network_menu("Rede " + country, "Opção: ", options2, text, false, true, 1);
    country_network_menu.render();

    vector<string> voltar_options = {"Voltar"};

    if (country_network_menu.optionIsSelected()) {
        switch (country_network_menu.getOption()) {
            case 0:
                consultar_rede();
                break;
            case 1: {
                vector<string> airports;
                for (auto &airport: directedNetwork) {
                    stringstream line;
                    line << airport.second.getCode() << " - " << airport.second.getName() << " - "
                         << airport.second.getCity();
                    airports.push_back(line.str());
                }
                Menu all_airports_menu("Aeroportos - " + country, "Opção: ", voltar_options, airports, false, true, 1);
                all_airports_menu.render();
                consultar_rede_pais(country);
                break;
            }
            case 2: {
                vector<string> important_airports;
                for (string airportCode: articulationPoints) {
                    Airport airport = m.getAirports().find(airportCode)->second;
                    stringstream line;
                    line << airport.getCode() << " - " << airport.getName() << " - " << airport.getCity();
                    important_airports.push_back(line.str());
                }
                Menu important_airports_menu("Aeroportos importantes - " + country, "Opção: ", voltar_options,
                                             important_airports, false, true, 1);
                important_airports_menu.render();
                consultar_rede_pais(country);
                break;
            }
            case 3: {
                vector<string> results;
                int max = 0;
                int count = 0;
                sort(stronglyConnectedComponents.begin(), stronglyConnectedComponents.end(),
                     [](const vector<string> &a, const vector<string> &b) {
                         return a.size() > b.size();
                     });
                for (auto component: stronglyConnectedComponents) {
                    string codes = "";
                    count = 0;
                    for (auto airportCode: component) {
                        Airport airport = m.getAirports().find(airportCode)->second;
                        count++;
                        codes += airport.getCode() + " <-> ";

                    }
                    if (count > max) max = count;
                    results.push_back(codes.erase(codes.length() - 5));
                }

                results.insert(results.begin(),
                               "O maior conjunto de aeroportos fortemente conexos tem " + to_string(max) +
                               " aeroportos.\n");

                Menu scc_menu("Componentes fortemente conexos - " + country, "Opção: ", voltar_options, results, false,
                              true, 1);
                scc_menu.render();
                consultar_rede_pais(country);
                break;

            }
            case 4: {
                string menutext =
                        "O diametro preciso é de " + to_string(m.getDiameter(directedNetwork, true)) + " aeroportos.";
                Menu precise_diameter("Diâmetro preciso - Rede " + country, "Opção: ", voltar_options, {menutext},
                                      false, true, 1);
                precise_diameter.render();
                consultar_rede_pais(country);
                break;
            }
            case 5: {
                string menutext =
                        "O diametro preciso em kms é de " + to_string(m.getWeightedDiameter(directedNetwork, true)) +
                        " kms.";
                Menu precise_diameter("Diâmetro preciso - Rede " + country, "Opção: ", voltar_options, {menutext},
                                      false, true, 1);
                precise_diameter.render();
                consultar_rede_pais(country);
                break;
            };
        }
    }
}

void menus::menu_results(unordered_map<string, Airport> &network) {
    vector<string> options = {"Voltar"};

    list<list<Flight*>> possible_paths = m.possiblePaths(travel_source_airports, travel_target_airports, network);

    vector<string> results;
    int count = 3;
    unordered_map<string, list<Flight*> const &> id_flights;
    for (const auto &trip: possible_paths) {
        //insert the trip to the map with count as the key
        id_flights.insert({to_string(count), trip});
        string each_trip = "";
        for (auto flight: trip) {
            each_trip += flight->getSource() + " -> " + flight->getTarget() + " (";
            vector<string> airlines = flight->getAirlines();
            for (int i = 0; i < airlines.size() - 1; i++) {
                each_trip += airlines[i] + ", ";
            }
            each_trip += airlines.back() += ") ";
        }
        results.push_back(to_string(count) + " - " + each_trip);
        count++;
    }
    vector<string> ids;
    for (int i = 3; i < count; i++) {
        ids.push_back(to_string(i));
    }
    Menu menu_results("Os melhores resultados", "Escolha uma opção ou o ID de um voo: ", options, results, ids, true, true, 2);
    menu_results.render();

    if (menu_results.optionIsSelected() && menu_results.getOption() == 0) {
        travel_source_airports.clear();
        travel_target_airports.clear();
        menu_viajar("De onde?");
    }


    string id = menu_results.getInput();

    string bilhete = "Comprou um bilhete para o seguinte voo: ";

    for(auto flight : id_flights.find(id)->second){
        auto airlines = flight->getAirlines();
        bilhete +=  flight->getSource() + " -> " + flight->getTarget() + " (" ;
        for (int i = 0; i < airlines.size() - 1; i++) {
            bilhete += airlines[i] + ", ";
        }
        bilhete += airlines.back() += ") ";
    }

    vector<string> bilhetes = {bilhete};

    vector<string> options_final = {"Voltar ao menu principal", "Viajar novamente"};
    Menu menu_final("Bilhetes", "Escolha uma opção: ", options_final, bilhetes, true, true, 1);
    menu_final.render();

    if(menu_final.optionIsSelected() && menu_final.getOption() == 0){
        mainMenu();
    }
    else if(menu_final.optionIsSelected() && menu_final.getOption() == 1){
        travel_source_airports.clear();
        travel_target_airports.clear();
        menu_viajar("De onde?");
    }

}

