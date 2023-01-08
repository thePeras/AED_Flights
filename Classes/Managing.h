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

    unordered_map<string, Airport> &getAirports();

    const unordered_map<string, vector<string>> &getCountryCities() const;

    /**
     * @brief Get the airports in a radius of a given location
     * Complexity: O(n)
     * @param location Center of the radius
     * @param radius Radius in kilometers
     * @return A vector of airports in the radius
     */
    const vector<Airport> getAirportsInRadius(Location location, double radius) const;

    vector<string> getAirportsInCity(string city, string country);

    vector<string> getAirportsInCountry(string country);

    unordered_map<string, Airport> getUndirectedGlobalNetwork();
    unordered_map<string, Airport> getAirlineNetwork(string airlineCode, bool directed);
    unordered_map<string, Airport> getCountryNetwork(string country, bool directed);

    list<list<Flight*>> possiblePaths(string source, string target, int maxNumFlights);

    list<list<Flight*>> possiblePaths(vector<string>& sources, vector<string>& targets, int maxNumFlights);

    list<list<Flight*>> possiblePaths(vector<string>& sources, vector<string>& targets, unordered_map<string, Airport> &network);

    list<list<Flight*>> possiblePaths(string source, string target, int maxNumFlights, set<string> &airlines);

    set<string> reachableAirports(string source, int maxNumFlights);

    string mostDistantCountry(string source, int maxNumFlights);

    /**
     * @brief Get the articulation points of a graph
     * Complexity: O(V + E)
     * @param network The undirected network (Graph) to be analyzed
     * @return A set of the articulation points of the network
     */
    set<string> getArticulationPoints(unordered_map<string, Airport> &network);

    /**
     * @brief A dfs based algorithm to find articulation points in an graph
     * Complexity: O(V + E)
     * @param source The source node
     * @param discovered The discovery time of the source node
     * @param low The lowest reachable node from the source node
     * @param parent The parent of the source node
     * @param articulationPoints The set of articulation points to be returned
     * @param network The undirected network (Graph) to be analyzed
     */
    void findArticulationPoints(string source, unordered_map<string, int> &discovered, unordered_map<string, int> &low, unordered_map<string, string> &parent, set<string> &articulationPoints, unordered_map<string, Airport> &network);

    /**
     * @brief BFS (Breadth First Search) algorithm
     * Complexity: O(V + E)
     * @param code The code of the airport to start the search
     * @param visited A map of visited airports
     * @param distances A map of distances from the source airport
     * @param graph The graph to search
     * @return The max diameter of the graph starting from the source
     */
    int bfs(string code, unordered_map<string, bool> visited, unordered_map<string, int> distances, const unordered_map<string, Airport> &graph);

    /**
     * @brief Calculates the max diameter of a graph
     * Complexity: O(V^2 + E)
     * @param graph The graph to search
     * @param precise If true, the algorithm will search for the exact diameter, if false, it will search for an approximation
     * @return The max diameter of the graph
     */
    int getDiameter(const unordered_map<string, Airport> &graph, bool precise);

    /**
     * @brief Calculates the weighted diameter of a graph starting from a given source
     * Complexity: O(V^2)
     * @param code The code of the airport to start the search
     * @param visited A map of visited airports
     * @param distances A map of distances from the source airport
     * @param graph The graph to search
     * @return The max weighted diameter of the graph of a given source
     */
    double dijkstra(string code, unordered_map<string, bool> visited, unordered_map<string, double> distances, const unordered_map<string, Airport> &graph);

    /**
     * @brief Calculates the max weighted diameter of a graph
     * Complexity: O(V^3)
     * @param graph The graph to search
     * @param precise If true, the algorithm will search for the exact diameter, if false, it will search for an approximation
     * @return The max weighted diameter of the graph
     */
    double getWeightedDiameter(const unordered_map<string, Airport> &graph, bool precise);

    /**
     * @brief DFS (Depth First Search) algorithm
     * Complexity: O(V + E)
     * @param code The code of the airport to start the search
     * @param visited A map of visited airports
     * @param graph The graph to search
     */
    void dfs(string code, unordered_map<string, bool> &visited, const unordered_map<string, Airport> &graph);

    /**
     * @brief Calculates the number of connected components in a graph
     * Complexity: O(V + E)
     * @param graph The graph to calculate the number of connected components
     * @return The number of connected components
     */
    int numberOfComponents(unordered_map<string, Airport> & graph);

    int numberOfDirectDestinations(string source, const unordered_map<string, Airport>& graph);

    vector<pair<string,int>> getTopAirports(int n, const unordered_map<string, Airport>& graph); // top aeroportos que permitem chegar a um maior número de destinos diferentes com um voo (dois aeroportos diferentes são considerados dois destinos diferentes)

private:
    void readAirlines();
    void readAirports();
    void readFlights();

    unordered_map<string, Airport> airports; /* Graph */
    unordered_map<string, Airline> airlines;
    unordered_map<string, vector<string>> country_cities;
    unordered_map<string, vector<string>> cities_airports;
    unordered_map<string, Airport> undirectedGlobalNetwork;

};

