#pragma once

#include "Location.h"
#include <string>
#include <list>

using namespace std;

class Flight;

class Airport {
public:
    Airport();
    Airport(string code, string name, string city, string country, Location location);
    Airport(string code, string name, string city, string country, float latitude, float longitude);

    const string &getName() const;

    string getCode() const;
    Location getLocation() const;
    void addFlight(Flight* flight);

    const list<Flight *> &getFlights() const;

private:
    string code;
    string name;
    string city;
    string country;
    Location location;
    list<Flight*> flights;
};

