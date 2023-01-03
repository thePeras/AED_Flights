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

    unordered_map<string, Airport> getUndirectedGlobalNetwork();
    unordered_map<string, Airport> getAirlineNetwork(string airlineCode, bool directed);

    list<list<Flight*>> possiblePaths(string source, string target, int maxNumFlights);

    list<list<Flight*>> possiblePaths(vector<string>& sources, vector<string>& targets, int maxNumFlights);

    list<list<Flight*>> possiblePaths(vector<string>& sources, vector<string>& targets, int maxNumFlights, set<string>& consideredAirlines);

    list<list<Flight*>> possiblePaths(string source, string target, int maxNumFlights, set<string> &airlines);

    set<string> reachableAirports(string source, int maxNumFlights);

    pair<string, int> mostDistantCountry(string source, int maxNumFlights);

    set<string> getArticulationPoints(unordered_map<string, Airport> &network, string source);
    void findArticulationPoints(string source, unordered_map<string, int> &disc, unordered_map<string, int> &low, unordered_map<string, string> &parent, set<string> &articulationPoints, unordered_map<string, Airport> &network);

    int getDiameter(const unordered_map<string, Airport> &graph, bool precise);
    int bfs(string code, unordered_map<string, bool> visited, unordered_map<string, int> distances, const unordered_map<string, Airport> &graph);
    double getWeightedDiameter(const unordered_map<string, Airport> &graph, bool precise);
    double dijkstra(string code, unordered_map<string, bool> visited, unordered_map<string, double> distances, const unordered_map<string, Airport> &graph);

private:
    void readAirlines();
    void readAirports();
    void readFlights();

    unordered_map<string, Airport> airports; /* Graph */
    unordered_map<string, Airline> airlines;
    unordered_map<string, vector<string>> country_cities;
    //TODO: unordered_map<string, vector<string>> cities_airports; where key = COUNTRY:CITY and value = vector of airports
    unordered_map<string, Airport> undirectedGlobalNetwork;

};

