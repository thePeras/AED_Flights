#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Airline.h"
#include "Airport.h"
#include <vector>
#include <stack>

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
    const vector<string> getAirportsInRadius(Location location, double radius) const;

    vector<string> getAirportsInCity(string city, string country);

    vector<string> getAirportsInCountry(string country);

    unordered_map<string, Airport> getUndirectedGlobalNetwork();

    unordered_map<string, Airport> getAirlineNetwork(string airlineCode, bool directed);

    unordered_map<string, Airport> getAirlinesNetwork(set<string> airlineCode, bool directed);

    unordered_map<string, Airport> getCountryNetwork(string country, bool directed);

    /**
     * @brief Get the shortest path between two airports in terms of flights
     * Complexity: O(V + E)
     * @param sources Source airports
     * @param targets Target airports
     * @param network graph of airports
     * @return A list of lists of flights
     */
    list<list<Flight *>> possiblePaths(vector<string> &sources, vector<string> &targets, unordered_map<string, Airport> &network);

    /**
     * @brief Get the reachable airports from a given airport up to a given number of flights
     * Complexity: O(V + E)
     * @param source Source airport
     * @param numFlights Number of flights
     * @return set of reachable airports
     */
    set<string> reachableAirports(string source, int numFlights);


    /**
     * @brief Get the most distant country in a number of flights
     * Complexity: O(V + E)
     * @param source
     * @param numFlights
     * @return
     */
    string mostDistantCountry(string source, int numFlights);


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
    void findArticulationPoints(string source, unordered_map<string, int> &discovered, unordered_map<string, int> &low,
                                unordered_map<string, string> &parent, set<string> &articulationPoints,
                                unordered_map<string, Airport> &network);


    vector<vector<string>> getStronglyConnectedComponentes(unordered_map<string, Airport> &network);

    /**
     * @brief A dfs based algorithm to find strongly connected components in a graph
     * Complexity: O(V + E)
     * @param source The source node
     * @param num The discovery time of the source node
     * @param low The lowest reachable node from the source node
     * @param s  The stack of the nodes in the current strongly connected component
     * @param inStack
     * @param stronglyConnectedAirports
     * @param network The undirected network (Graph) to be analyzed
     */
    void findStronglyConnectAirports(const string source, unordered_map<string, int> &num,
                                     unordered_map<string, int> &low, stack<string> &s,
                                     unordered_map<string, bool> &inStack,
                                     vector<vector<string>> &stronglyConnectedAirports,
                                     unordered_map<string, Airport> &network);


    /**
     * @brief BFS (Breadth First Search) algorithm
     * Complexity: O(V + E)
     * @param code The code of the airport to start the search
     * @param visited A map of visited airports
     * @param distances A map of distances from the source airport
     * @param graph The graph to search
     * @return The max diameter of the graph starting from the source
     */
    int bfs(string code, unordered_map<string, bool> visited, unordered_map<string, int> distances,
            const unordered_map<string, Airport> &graph);

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
    double dijkstra(string code, unordered_map<string, bool> visited, unordered_map<string, double> distances,
                    const unordered_map<string, Airport> &graph);

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
    int numberOfComponents(unordered_map<string, Airport> &graph);

    int numberOfDirectDestinations(string source, const unordered_map<string, Airport> &graph);

    vector<pair<string, int>> getTopAirports(int n, const unordered_map<string, Airport> &graph);

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

