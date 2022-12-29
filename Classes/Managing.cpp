#define AIRLINES_FILE "../Data/airlines.csv"
#define AIRPORTS_FILE "../Data/airports.csv"
#define FLIGHTS_FILE "../Data/flights.csv"

#include "Managing.h"
#include "Airline.h"
#include "Flight.h"
#include "Airport.h"
#include "Location.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <limits>
#include <queue>

Managing::Managing() {
}

void Managing::readFiles() {
    readAirlines();
    readAirports();
    readFlights();
}

void Managing::readAirlines() {
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
            airlines.insert({code, airline});
        }
        file.close();
    }
}

void Managing::readAirports() {
    string line;
    ifstream file(AIRPORTS_FILE);
    getline(file, line);
    while (getline(file, line)) {
        istringstream ss (line);
        string code, name, city, country;
        float latitude, longitude;
        getline(ss, code, ',');
        if (code.empty()) break;
        getline(ss, name, ',');
        getline(ss, city, ',');
        getline(ss, country, ',');
        ss >> latitude;
        ss.ignore();
        ss >> longitude;

        country_cities[country].push_back(city);

        Airport airport(code, name, city, country, *new Location(latitude, longitude));
        airports.insert({code, airport});
    }
    file.close();
}

void Managing::readFlights() {
    string line;
    ifstream file(FLIGHTS_FILE);

    getline(file, line);
    while (getline(file, line)) {
        istringstream ss (line);
        string origin, destination, airline;
        getline(ss, origin, ',');
        if (origin.empty()) break;
        getline(ss, destination, ',');
        getline(ss, airline);
        ss.ignore();

        // Basically adding edges to the graph
        float distance = airports[origin].getLocation().distance(airports[destination].getLocation());
        airports[origin].addFlight(new Flight(origin, destination, airline, distance));

    }
    file.close();
}

const unordered_map<string, Airline> &Managing::getAirlines() const {
    return airlines;
}

const unordered_map<string, Airport> &Managing::getAirports() const {
    return airports;
}

const unordered_map<string, vector<string>> &Managing::getCountryCities() const {
    return country_cities;
}

const vector<Airport> Managing::getAirportsInRadius(Location location, double radius) const {
    vector<Airport> airportsInRadius;
    for (auto airport : airports) {
        if (airport.second.getLocation().distance(location) <= radius) {
            airportsInRadius.push_back(airport.second);
        }
    }
    return airportsInRadius;
}

vector<string> Managing::getAirportsInCity(string city, string country) {
    vector<string> airportsInCity;
    for (auto airport : airports) {
        if (airport.second.getCity() == city && airport.second.getCountry() == country) {
            airportsInCity.push_back(airport.first);
        }
    }
    return airportsInCity;
}

vector<string> Managing::getAirportsInCountry(string country) {
    vector<string> airportsInCountry;
    for (auto airport : airports) {
        if (airport.second.getCountry() == country) {
            airportsInCountry.push_back(airport.second.getName() + " (" + airport.first + ")");
        }
    }
    return airportsInCountry;
}

list<list<Flight *>> Managing::possiblePaths(string source, string target, int maxNumFlights) {
    list<list<Flight*>> paths;
    unordered_map<string, bool> visited;

    list<list<Flight*>> possiblePaths;

    paths.push_back({});

    while (!paths.empty()) {
        list<Flight*> path = paths.front();

        paths.pop_front();

        string lastAirport = (path.empty()) ? source : path.back()->getTarget();

        if (lastAirport == target) {
            possiblePaths.push_back(path);
            continue;
        }

        visited[lastAirport] = true;

        Airport lastAirportObj = airports[lastAirport];

        for (Flight* flight : lastAirportObj.getFlights()) {
            if (!visited[flight->getTarget()]) {
                list<Flight*> newPath = path;
                newPath.push_back(flight);

                if (newPath.size() > maxNumFlights) continue;

                paths.push_back(newPath);
            }
        }

    }
    return possiblePaths;
}

