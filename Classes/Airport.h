#pragma once

#include "Location.h"
#include <string>

using namespace std;

class Airport {
    private:
    string code;
    string name;
    string city;
    string country;
    Location location;
public:
    Airport();
    Airport(string code, string name, string city, string country, Location location);
    Airport(string code, string name, string city, string country, float latitude, float longitude);
    string getCode() const;
    Location getLocation() const;
};

