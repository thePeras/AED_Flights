
#include "Graph.h"

Graph::Graph(int num, bool dir) : n(num), nodes(num+1) {}

void Graph::addEdge(int src, int dest, int weight) { //TODO: change to add flight
    if (src<1 || src>n || dest<1 || dest>n) return;

    nodes[src].adj.push_back({dest, weight});

    i f (!hasDir) nodes[dest].adj.push_back({src, weight});
}
