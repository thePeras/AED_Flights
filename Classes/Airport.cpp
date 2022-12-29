#include "Airport.h"

using namespace std;

Airport::Airport() {
    code = "";
    name = "";
    city = "";
    country = "";
    location = Location();
}

Airport::Airport(string code, string name, string city, string country, Location location) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    this->location = location;
}

Airport::Airport(string code, string name, string city, string country, float latitude, float longitude) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    this->location = Location(latitude, longitude);
}

string Airport::getCode() const {
    return code;
}

string Airport::getName() const {
    return name;
}

Location Airport::getLocation() const {
    return location;
}

void Airport::addFlight(Flight *flight) {
    flights.push_back(flight);
}

const list<Flight *> &Airport::getFlights() const {
    return flights;
}

const string &Airport::getCity() const {
    return city;
}

const string &Airport::getCountry() const {
    return country;
}
