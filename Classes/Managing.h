#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Airline.h"
#include "Comparators.cpp"
#include "Airport.h"
#include "Graph.h"

using namespace std;

class Managing {
public:

    Managing();

    /**
     * @brief
     * Lê todos os ficheiros: estudantes, horários, classes/UCs, requests e rejected requests.
     */
    void readFiles();

    const unordered_map<string, Airline> &getAirlines() const;

    const unordered_map<string, Airport> &getAirports() const;

    const unordered_map<string, vector<string>> &getCountryCities() const;

    const Graph &getGraph() const;

private:
    //TODO: Add sets, lists, vectors (or whatever we want to use) to store the data

    void readAirlines();
    void readAirports();
    void readFlights();
    Graph graph;
    unordered_map<string, Airline> airlines;
    unordered_map<string, Airport> airports;
    unordered_map<string, vector<string>> country_cities;

};

