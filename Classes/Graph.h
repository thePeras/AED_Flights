#pragma once

class Graph {
    struct Edge { //TODO: replace with Flight
        int dest;
        int weight; // this is distance or time
    }

    struct Node {
        list <Edge> adj; // only correct directed edges, flights departure from this airport
        string airport; // airport id
    };

    int n;
    vector <Node> nodes; // basically is a vector of all airports

    public:
        Graph(int nodes);

        void addEdge(int src, int dest, int weight = 1);
    };
}
