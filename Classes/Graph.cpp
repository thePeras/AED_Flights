
#include "Graph.h"

void Graph::addEdge(int src, int dest, int weight) { //TODO: change to add flight
    bool hasDir = false;
    if (src<1 || src>n || dest<1 || dest>n) return;

    nodes[src].adj.push_back({dest, weight});

    if (!hasDir) nodes[dest].adj.push_back({src, weight});
}
