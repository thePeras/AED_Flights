#define AIRLINES_FILE "../Data/airlines.csv"
#define AIRPORTS_FILE "../Data/airports.csv"
#define FLIGHTS_FILE "../Data/flights.csv"

#include "Managing.h"
#include "Airline.h"
#include <algorithm>
#include <sstream>
#include <fstream>


Managing::Managing() {

}

void Managing::readFiles() {
    readAirlines();
    //readAirports();
    //readFlights();
}

void Managing::readAirlines() {
    set<Airline, airlineComp> airlines = {};
    ifstream file(AIRLINES_FILE);

    if (file.is_open()) {
        file.ignore(49, '\n');
        while (!file.eof()) {

            string code;
            string name;
            string callsign;
            string country;

            getline(file, code, ',');
            if (code.empty()) break;
            getline(file, name, ',');
            getline(file, callsign, ',');
            getline(file, country);

            Airline airline(code, name, callsign, country);
            airlines.insert(airline);

        }
        file.close();
        this->airlines = airlines;
    }
}

const set<Airline, airlineComp> &Managing::getAirlines() const {
    return airlines;
}

void Managing::setAirlines(const set<Airline, airlineComp> &airlines) {
    Managing::airlines = airlines;
}
