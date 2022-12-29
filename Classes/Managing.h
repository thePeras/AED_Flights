#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Airline.h"
#include "Comparators.cpp"
#include "Airport.h"
#include <vector>

using namespace std;

class Managing {
public:

    Managing();

    void readFiles();

    const unordered_map<string, Airline> &getAirlines() const;

    const unordered_map<string, Airport> &getAirports() const;

    const vector<Airport> getAirportsInRadius(Location location, double radius) const;

    const unordered_map<string, vector<string>> &getCountryCities() const;

    vector<string> getAirportsInCity(string city, string country);

    vector<string> getAirportsInCountry(string country);

    pair<float, vector<Flight*>> dijkstra(string origin, string destination);


private:
    //TODO: Add sets, lists, vectors (or whatever we want to use) to store the data

    void readAirlines();
    void readAirports();
    void readFlights();

    unordered_map<string, Airport> airports; /* Graph */
    unordered_map<string, Airline> airlines;
    unordered_map<string, vector<string>> country_cities;

};

