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

list<list<Flight*>> Graph::possiblePaths(string source, string target, int maxNumFlights) {
    list<list<Flight*>> paths;

    unordered_map<string, bool> visited;

    list<list<Flight*>> possiblePaths;

    paths.push_back({});

    while (!paths.empty()) {
        list<Flight*> path = paths.front();

        paths.pop_front();

        string lastAirport = (path.empty()) ? source : path.back()->getTarget();

        if (lastAirport == target) {
            possiblePaths.push_back(path);
            continue;
        }

        visited[lastAirport] = true;

        Airport* lastAirportObj = airports[lastAirport];

        for (Flight* flight : lastAirportObj->getFlights()) {
            if (!visited[flight->getTarget()]) {
                list<Flight*> newPath = path;
                newPath.push_back(flight);

                if (newPath.size() > maxNumFlights) continue;

                paths.push_back(newPath);
            }
        }

    }
    return possiblePaths;
}
