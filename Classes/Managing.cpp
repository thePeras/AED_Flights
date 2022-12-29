#define AIRLINES_FILE "../Data/airlines.csv"
#define AIRPORTS_FILE "../Data/airports.csv"
#define FLIGHTS_FILE "../Data/flights.csv"

#include "Managing.h"
#include "Airline.h"
#include "Flight.h"
#include "Airport.h"
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
        airports[origin].addFlight(new Flight (airports[origin], airports[destination], airline));
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

pair<float, vector<Flight *>> Managing::dijkstra(string origin, string destination) {
    unordered_map<string, float> distances;

    unordered_map<string, string> previous;

    for (auto airport : airports) {
        distances[airport.first] = numeric_limits<float>::infinity();
    }

    distances[origin] = 0;


    priority_queue<pair<float, string>> pq;

    pq.emplace(0, origin);

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        if (current.second == destination) break;

        Airport &currentAirport = airports.find(current.second)->second;

        for (Flight *flight : currentAirport.getFlights()) {
            float distance = flight->getDistance() + current.first;

            string next = flight->getTarget();
            if (distances[next] > distance) {
                distances[next] = distance;
                previous[next] = current.second;
                pq.emplace(distance, next);
            }
        }
    }

    vector<Flight *> path;
    string current = destination;
    while (current != previous[current]) {
        Airport &currentAirport = airports.find(current)->second;
        for (Flight *flight: currentAirport.getFlights()) {
            if (flight->getTarget() == previous[current] && flight->getSource() == current) {
                path.push_back(flight);
                break;
            }
        }
        current = previous[current];
    }
    reverse(path.begin(), path.end());
    return {distances[destination], path};
}

