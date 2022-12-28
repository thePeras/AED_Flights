#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include "Airport.h"

using namespace std;

class Graph {
public:
    unordered_map<string, Airport*> airports; // Nodes
    int n;

    Graph() {};
    explicit Graph(int num) : n(num), airports(num+1) {};
    void addAirport(Airport* airport);
    void dfs (Airport* airport);
    void dfsHelper(Airport *airport, unordered_map<string, bool> &visited);
    void bfs (Airport* airport);
    list<list<Flight*>> possiblePaths(string source, string target, int maxNumFlights);
};
