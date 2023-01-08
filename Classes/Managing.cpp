#define AIRLINES_FILE "../Data/airlines.csv"
#define AIRPORTS_FILE "../Data/airports.csv"
#define FLIGHTS_FILE "../Data/flights.csv"

#include "Managing.h"
#include "Airline.h"
#include "Flight.h"
#include "Airport.h"
#include "Location.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <queue>

Managing::Managing() {
}

void Managing::readFiles() {
    readAirlines();
    readAirports();
    readFlights();
}

void Managing::readAirlines() {
    ifstream file(AIRLINES_FILE);

    if (file.is_open()) {
        file.ignore(49, '\n');
        while (!file.eof()) {

            string code;
            string name;
            string callsign;
            string country;

            getline(file, code, ',');
            if (code.empty()) break;
            getline(file, name, ',');
            getline(file, callsign, ',');
            getline(file, country);

            Airline airline(code, name, callsign, country);
            airlines.insert({code, airline});
        }
        file.close();
    }
}

void Managing::readAirports() {
    string line;
    ifstream file(AIRPORTS_FILE);
    getline(file, line);
    while (getline(file, line)) {
        istringstream ss(line);
        string code, name, city, country;
        float latitude, longitude;
        getline(ss, code, ',');
        if (code.empty()) break;
        getline(ss, name, ',');
        getline(ss, city, ',');
        getline(ss, country, ',');
        ss >> latitude;
        ss.ignore();
        ss >> longitude;

        Airport airport(code, name, city, country, Location(latitude, longitude));
        Airport undirectedAirport(code, name, city, country, Location(latitude, longitude));

        airports.insert({code, airport});
        undirectedGlobalNetwork.insert({code, undirectedAirport});

        country_cities[country].push_back(city);

        string countryCityKey = country + ":" + city;
        cities_airports[countryCityKey].push_back(code);
    }
    file.close();
}

void Managing::readFlights() {
    string line;
    ifstream file(FLIGHTS_FILE);
    unordered_map<string, Flight*> flights;

    getline(file, line);
    while (getline(file, line)) {
        istringstream ss(line);
        string origin, destination, airline;
        getline(ss, origin, ',');
        if (origin.empty()) break;
        getline(ss, destination, ',');
        getline(ss, airline);
        ss.ignore();

        // Basically adding edges to the graph
        float distance = airports[origin].distance(&airports[destination]);
        Flight *flight = new Flight(origin, destination, distance);

        auto findFlight = flights.find(origin + ":" + destination);
        if (findFlight == flights.end()) {
            flights.insert({origin + ":" + destination, flight});
            flight->addAirline(airline);
            // direct graph
            airports[origin].addFlight(flight);
            //undirected graph
            undirectedGlobalNetwork[origin].addFlight(flight);
            undirectedGlobalNetwork[destination].addFlight(flight);

            airlines[airline].addFlight(flight);

        }
        else {
            findFlight->second->addAirline(airline);
        }

        airlines[airline].addAirport(origin);
        airlines[airline].addAirport(destination);

    }
    file.close();
}

const unordered_map<string, Airline> &Managing::getAirlines() const {
    return airlines;
}

unordered_map<string, Airport> &Managing::getAirports(){
    return this->airports;
}

const unordered_map<string, vector<string>> &Managing::getCountryCities() const {
    return country_cities;
}

const vector<Airport> Managing::getAirportsInRadius(Location location, double radius) const {
    vector<Airport> airportsInRadius;
    for (auto airport: airports) {
        if (airport.second.getLocation().distance(location) <= radius) {
            airportsInRadius.push_back(airport.second);
        }
    }
    return airportsInRadius;
}

vector<string> Managing::getAirportsInCity(string city, string country) {
    return cities_airports[country + ":" + city];
}

vector<string> Managing::getAirportsInCountry(string country) {
    vector<string> airportsInCountry;

    for(string city : country_cities[country]){
        vector<string> airportsInCity = cities_airports[country + ":" + city];
        airportsInCountry.insert(airportsInCountry.end(), airportsInCity.begin(), airportsInCity.end());
    }

    return airportsInCountry;
}

unordered_map<string, Airport> Managing::getUndirectedGlobalNetwork() {
    return undirectedGlobalNetwork;
}

unordered_map<string, Airport> Managing::getAirlineNetwork(string airlineCode, bool directed) {
    unordered_map<string, Airport> newNetwork;

    Airline airline = airlines[airlineCode];

    for(string airportCode : airline.getAirports()){
        Airport airport = new Airport(airports[airportCode]);
        newNetwork[airportCode] = airport;
    }

    for (Flight* flight : airline.getFlights()) {
        string source = flight->getSource();
        string target = flight->getTarget();

        newNetwork[source].addFlight(flight);
        if(!directed) newNetwork[target].addFlight(flight);
    }

    return newNetwork;
}

unordered_map<string, Airport> Managing::getCountryNetwork(string country, bool directed) {
    unordered_map<string, Airport> newNetwork;

    for(string airportCode : getAirportsInCountry(country)){
        Airport new_airport = new Airport(airports[airportCode]);
        newNetwork[airportCode] = new_airport;
    }

    //adding the flights
    for(auto pair : newNetwork){
        Airport airportWithAllFlights = airports[pair.first];

        for(Flight* flight : airportWithAllFlights.getFlights()){

            //Adding just the flights in the same country
            string target = flight->getTarget();
            if(airports[target].getCountry() == country){
                newNetwork[pair.first].addFlight(flight);
                if(!directed) newNetwork[target].addFlight(flight);
            }
        }
    }

    return newNetwork;
}

list<list<Flight *>> Managing::possiblePaths(string source, string target, int maxNumFlights) {
    queue<list<Flight *>> paths;
    unordered_map<string, bool> visited;

    list<list<Flight *>> possiblePaths;

    paths.push({});

    while (!paths.empty()) {
        list<Flight *> path = paths.front();

        paths.pop();

        string lastAirport = (path.empty()) ? source : path.back()->getTarget();

        if (lastAirport == target) {
            possiblePaths.push_back(path);
            continue;
        }

        visited[lastAirport] = true;

        Airport lastAirportObj = airports[lastAirport];

        for (Flight *flight: lastAirportObj.getFlights()) {
            if (!visited[flight->getTarget()]) {
                list<Flight *> newPath = path;
                newPath.push_back(flight);

                if (newPath.size() > maxNumFlights) continue;

                paths.push(newPath);
            }
        }

    }
    return possiblePaths;
}

list<list<Flight*>> Managing::possiblePaths(string source, string target, int maxNumFlights, set<string>& consideredAirlines) {
    queue<list<Flight *>> paths;
    unordered_map<string, bool> visited;

    list<list<Flight *>> possiblePaths;

    paths.push({});

    while (!paths.empty()) {
        list<Flight *> path = paths.front();

        paths.pop();

        string lastAirport = (path.empty()) ? source : path.back()->getTarget();

        if (lastAirport == target) {
            possiblePaths.push_back(path);
            continue;
        }

        visited[lastAirport] = true;

        Airport lastAirportObj = airports[lastAirport];

        for (Flight *flight: lastAirportObj.getFlights()) {
            /* TO CHANGE
            if (consideredAirlines.find(flight->getAirline()) == consideredAirlines.end()) continue;
             */
            if (!visited[flight->getTarget()]) {
                list<Flight *> newPath = path;
                newPath.push_back(flight);

                if (newPath.size() > maxNumFlights) continue;

                paths.push(newPath);
            }
        }

    }
    return possiblePaths;
}

list<list<Flight *>> Managing::possiblePaths(vector<string>& sources, vector<string>& targets, int maxNumFlights) {
    queue<list<Flight *>> paths;
    unordered_map<string, bool> visited;

    list<list<Flight *>> possiblePaths;

    for (string source : sources) {
        for (string target : targets) {
            visited.clear();
            paths.push({});
            while (!paths.empty()) {
                list<Flight *> path = paths.front();
                paths.pop();
                string lastAirport = (path.empty()) ? source : path.back()->getTarget();
                if (lastAirport == target) {
                    possiblePaths.push_back(path);
                    continue;
                }
                visited[lastAirport] = true;
                Airport lastAirportObj = airports[lastAirport];
                for (Flight *flight: lastAirportObj.getFlights()) {
                    if (!visited[flight->getTarget()]) {
                        list<Flight *> newPath = path;
                        newPath.push_back(flight);
                        if (newPath.size() > maxNumFlights) continue;
                        paths.push(newPath);
                    }
                }
            }
        }
    }
    return possiblePaths;
}

list<list<Flight *>> Managing::possiblePaths(vector<string>& sources, vector<string>& targets, int maxNumFlights, unordered_map<string, Airport> &network) {
    unordered_map<string, bool> visited;
    visited.clear();

    queue<queue<Airport>> solutions;

    queue<queue<Airport>> airportsPaths;

    for (string source : sources) {
        queue<Airport> airports;
        airports.push(network[source]);
        airportsPaths.push(airports);

        visited[source] = true;
    }


    while (!airportsPaths.empty()) {

        queue<Airport> path = airportsPaths.front();
        airportsPaths.pop();

        Airport airport = path.back();

        if (find(targets.begin(), targets.end(), airport.getCode()) != targets.end()) {
            solutions.push(path);
            continue;
        }

        for (Flight *flight: airport.getFlights()) {
            if (!visited[flight->getTarget()]) {
                queue<Airport> newPath = path;
                newPath.push(network[flight->getTarget()]);
                if (newPath.size() > maxNumFlights+1) continue;
                airportsPaths.push(newPath);
                visited[flight->getTarget()] = true;
            }
        }
    }

    //Converting the queue of queues of airports to a list of lists of flights
    //For each pair of airports a, b. Find all flights from a to b (to see all the airlines)

    return list<list<Flight *>>();
}

set<string> Managing::reachableAirports(string source, int maxNumFlights) {
    set<string> reachableAirports;
    unordered_map<string, bool> visited;

    queue<pair<string, int>> paths;
    paths.push({source, 0});

    while (!paths.empty()) {
        pair<string, int> path = paths.front();
        paths.pop();

        string lastAirport = path.first;
        int numFlights = path.second;

        if (visited[lastAirport]) continue;
        visited[lastAirport] = true;
        reachableAirports.insert(lastAirport);

        Airport lastAirportObj = airports[lastAirport];

        for (Flight *flight: lastAirportObj.getFlights()) {
            if (numFlights + 1 > maxNumFlights) continue;

            paths.push({flight->getTarget(), numFlights + 1});
        }
    }
    return reachableAirports;
}

string Managing::mostDistantCountry(string source, int maxNumFlights) {
    string distantCountry = "";
    double maxDistance = 0;
    Airport sourceAirport = this->airports[source];
    set<string> airports = reachableAirports(source, maxNumFlights);
    for(string airportCode : airports) {
        Airport airport = this->airports[airportCode];
        double distance = sourceAirport.distance(&airport);
        if(distance > maxDistance){
            maxDistance = distance;
            distantCountry = airport.getCountry();
        }
    }
    return distantCountry;
}

set<string> Managing::getArticulationPoints(unordered_map<string, Airport> &network){
    unordered_map<string, int> discovered;
    unordered_map<string, int> low;
    unordered_map<string, string> parent;
    set<string> articulationPoints;

    for (auto &node : network){
        discovered[node.first] = -1;
        low[node.first] = -1;
    }

    for (auto &node : network){
        if(discovered[node.first] == -1){
            findArticulationPoints(node.first, discovered, low, parent, articulationPoints, network);
        }
    }

    return articulationPoints;
}

//tarjan's algorithm
void Managing::findArticulationPoints(
        string source, unordered_map<string, int> &discovered, unordered_map<string, int> &low,
        unordered_map<string, string> &parent, set<string> &articulationPoints, unordered_map<string, Airport> &network) {

    static int time = 0;
    int children = 0;

    discovered[source] = low[source] = ++time;

    Airport sourceAirportObj = network[source];

    for (Flight *flight: sourceAirportObj.getFlights()) {
        string target = flight->getTarget();

        if (discovered[target] == -1) {
            children++;
            parent[target] = source;
            findArticulationPoints(target, discovered, low, parent, articulationPoints, network);

            low[source] = min(low[source], low[target]);

            // Source is root
            if (parent[source] == "" && children > 1) {
                articulationPoints.insert(source);
            }

            // A component will be separated
            if (parent[source] != "" && low[target] >= discovered[source]) {
                articulationPoints.insert(source);
            }
        } else if (target != parent[source]) {
            low[source] = min(low[source], discovered[target]);
        }
    }
}

int Managing::bfs(string code, unordered_map<string, bool> visited, unordered_map<string, int> distances, const unordered_map<string, Airport> &graph) {
    int diameter = 0;
    queue<string> q;
    q.push(code);
    visited[code] = true;
    distances[code] = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        for (Flight* flight : graph.find(current)->second.getFlights()) {
            string w = flight->getTarget();
            if (!visited[w]) {
                q.push(w);
                visited[w] = true;
                distances[w] = distances[current] + 1;
                diameter = max(diameter, distances[w]);
            }
        }
    }
    return diameter;
}

int Managing::getDiameter(const unordered_map<string, Airport>& graph, bool precise) {
    int diameter = 0;
    unordered_map<string, bool> visited;
    unordered_map<string, int> distances;
    int k = 0;

    for (auto& [code, airport] : graph) {
        visited[code] = false;
        distances[code] = -1;
    }

    for (auto& [code, airport] : graph) {
        diameter = max(bfs(code, visited, distances, graph), diameter);
        if(!precise and k++ == 12) break;
    }
    return diameter;
}

double Managing::dijkstra(string code, unordered_map<string, bool> visited, unordered_map<string, double> distances, const unordered_map<string, Airport> &graph) {
    double diameter = 0;
    queue<string> q;
    q.push(code);
    visited[code] = true;
    distances[code] = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        for (Flight* flight : graph.find(current)->second.getFlights()) {
            string w = flight->getTarget();
            if (!visited[w]) {
                q.push(w);
                visited[w] = true;
                distances[w] = distances[current] + flight->getDistance();
                diameter = max(diameter, distances[w]);
            }
        }
    }

    return diameter;
}

double Managing::getWeightedDiameter(const unordered_map<std::string, Airport> &graph, bool precise) {
    double diameter = 0;
    unordered_map<string, bool> visited;
    unordered_map<string, double> distances;
    int k = 0;

    for (auto& [code, airport] : graph) {
        visited[code] = false;
        distances[code] = -1;
    }

    for (auto& [code, airport] : graph) {
        diameter = max(dijkstra(code, visited, distances, graph), diameter);
        if(!precise and k++ == 14) break;
    }
    return diameter;
}

void Managing::dfs(string code, unordered_map<string, bool> &visited, const unordered_map<string, Airport> &graph) {
    visited[code] = true;

    for (Flight* flight : graph.find(code)->second.getFlights()) {
        string w = flight->getTarget();
        if (!visited[w]) {
            dfs(w, visited, graph);
        }
    }
}

int Managing::numberOfComponents(unordered_map<string, Airport>& network) {
    unordered_map<string, bool> visited;

    for (auto it = network.begin(); it != network.end(); it++) {
        visited[it->first] = false;
    }

    int nComponents = 0;

    for (auto it = network.begin(); it != network.end(); it++) {
        if (!visited[it->first]) {
            nComponents++;
            dfs(it->first, visited, network);
        }
    }

    return nComponents;
}

int Managing::numberOfDirectDestinations(string source, const unordered_map<string, Airport>& graph) {
    return graph.find(source)->second.getFlights().size();
}

vector<pair<string, int>> Managing::getTopAirports(int n, const unordered_map<string, Airport>& graph) {
    vector<pair<string,int>> topAirports;
    for (auto &airport : graph) {
        topAirports.push_back(make_pair(airport.first, numberOfDirectDestinations(airport.first, graph)));
    }
    sort(topAirports.begin(), topAirports.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        return a.second > b.second;
    });
    return vector<pair<string, int>>(topAirports.begin(), topAirports.begin() + n);
}
