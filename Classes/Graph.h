#pragma once

#include <list>
#include <string>
#include <vector>

using namespace std;



class Graph {

    struct Edge { //TODO: replace with Flight
        int dest;
        int weight; // this is distance or time
    };

    struct Node {
        list<Edge> adj; // only correct directed edges, flights departure from this airport
        string airport; // airport id
        bool visited; // verifies if the node has been visited on a search
    };

    int n;
    vector <Node> nodes; // basically is a vector of all airports

    public:
        Graph(int num) : n(num), nodes(num+1) {};
        void addEdge(int src, int dest, int weight = 1);
    ;
};
