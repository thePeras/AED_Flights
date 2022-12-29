#pragma once

#include "Location.h"
#include <string>
#include <list>
#include <set>

using namespace std;

class Flight;

class Airport {
public:
    Airport();
    Airport(string code, string name, string city, string country, Location location);
    Airport(string code, string name, string city, string country, float latitude, float longitude);
    string getCode() const;
    string getName() const;
    Location getLocation() const;
    void addFlight(Flight* flight);

    const string &getCity() const;

    const string &getCountry() const;

    const list<Flight *> &getFlights() const;

    const set<string> getAirlines() const;
private:
    string code;
    string name;
    string city;
    string country;
    Location location;
    list<Flight*> flights;
};

