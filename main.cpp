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

void aeroporto_input(){
    MenuTwo menu_aeroporto_input("Aeroporto", "digite o nome do aeroporto", {}, {"OPO", "JFK"});
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

    MenuTwo menu_latitude("Viajar - de onde?", "latitude", options_viajar);
    menuStack.push(menu_latitude);
    menu_latitude.render();
    string latitude = menu_latitude.getInput();
    while(!latitudeIsValid(latitude)){
        menu_latitude.render();
        latitude = menu_latitude.getInput();
    }

    MenuTwo menu_longitude("Viajar - de onde?", "longitude", options_viajar);
    menu_longitude.render();
    string longitude = menu_longitude.getInput();
    while(!longitudeIsValid(longitude)){
        menu_longitude.render();
        longitude = menu_longitude.getInput();
    }

    MenuTwo menu_radius("Viajar - de onde?", "raio", options_viajar);
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

void menu_viajar(){
    vector<MenuOption> options_viajar = {
            {"Voltar", back_action},
            {"Aeroporto (TAG)", aeroporto_input},
            {"País", []() {}},
            {"Coordenadas", menu_coordenadas},
    };

    MenuTwo menu_viajar("Viajar", "de onde? ", options_viajar, {});
    menuStack.push(menu_viajar);

    menu_viajar.render();
}

int main(){
    m.readFiles();

    Graph g = m.getGraph();
    // perform bfs on the graph and print the airports
    g.bfs(g.airports["OPO"]);

    vector<MenuOption> options = {
            {"Viajar", menu_viajar},
            {"Consultar Aeroporto", []() {cout << "Remove Location" << endl;}},
            {"Os meus bilhetes", []() {cout << "Edit Location" << endl;}},
    };

    MenuTwo MainMenu("Inicio", "Escolha uma opção", options, {}, false);
    menuStack.push(MainMenu);
    MainMenu.render();



    return 0;
}