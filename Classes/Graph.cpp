#include "Airport.h"
#include "Flight.h"
#include "Graph.h"
#include <queue>
#include <set>

void Graph::addAirport(Airport *airport) {
    airports.insert({airport->getCode(), airport});

}

void Graph::dfsHelper(Airport *airport, unordered_map<string, bool>& visited) {
    visited[airport->getCode()] = true;

    cout << airport->getCode() << " ";

    for (const auto &flight : airport->getFlights()) {
        if (!visited[flight->getTarget()]) {
            dfsHelper(airports[flight->getTarget()], visited);
        }
    }
}

void Graph::bfs(Airport *airport) {
    unordered_map<string, bool> visited;

    for (const auto &pair : airports) {
        visited[pair.first] = false;
    }

    queue<Airport*> q;

    visited[airport->getCode()] = true;

    q.push(airport);

    while (!q.empty()) {
        Airport *a = q.front();
        q.pop();

        cout << a->getCode() << " ";

        for (const auto &flight : a->getFlights()) {
            if (!visited[flight->getTarget()]) {
                visited[flight->getTarget()] = true;
                q.push(airports[flight->getTarget()]);
            }
        }

    }
}

void Graph::dfs(Airport *airport) {
    unordered_map<string, bool> visited;
    for (const auto &pair : airports) {
        visited[pair.first] = false;
    }
    dfsHelper(airport, visited);
}

list<list<string>> Graph::possiblePaths(string source, string target) {
    list<list<string>> paths;

    unordered_map<string, bool> visited;

    list<list<string>> shortestPaths;

    paths.push_back({source});

    while (!paths.empty()) {
        list<string> path = paths.front();
        paths.pop_front();

        string lastAirport = path.back();

        if (lastAirport == target) {
            shortestPaths.push_back(path);
            continue;
        }

        visited[lastAirport] = true;

        Airport* lastAirportObj = airports[lastAirport];

        for (Flight* flight : lastAirportObj->getFlights()) {
            if (!visited[flight->getTarget()]) {
                list<string> newPath = path;
                newPath.push_back(flight->getTarget());
                paths.push_back(newPath);
            }
        }

    }

    return shortestPaths;
}
