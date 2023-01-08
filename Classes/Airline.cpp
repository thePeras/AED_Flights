#include "Airline.h"

using namespace std;

Airline::Airline() {
    code = "";
    name = "";
    callSign = "";
    country = "";
    flights = {};
    airports = {};
}

Airline::Airline(string code, string name, string callSign, string country) {
    this->code = code;
    this->name = name;
    this->callSign = callSign;
    this->country = country;
    this->flights = {};
    this->airports = {};
}

void Airline::addFlight(Flight *flight) {
    flights.push_back(flight);
}

void Airline::addAirport(string airport) {
    airports.insert(airport);
}

const string &Airline::getCode() const {
    return code;
}

void Airline::setCode(const string &code) {
    Airline::code = code;
}

const string &Airline::getName() const {
    return name;
}

void Airline::setName(const string &name) {
    Airline::name = name;
}

const string &Airline::getCallSign() const {
    return callSign;
}

void Airline::setCallSign(const string &callSign) {
    Airline::callSign = callSign;
}

const string &Airline::getCountry() const {
    return country;
}

void Airline::setCountry(const string &country) {
    Airline::country = country;
}

const vector<Flight *> &Airline::getFlights() const {
    return flights;
}

void Airline::setFlights(const vector<Flight *> &flights) {
    Airline::flights = flights;
}

const set<string> &Airline::getAirports() const {
    return airports;
}

void Airline::setAirports(const set<string> &airports) {
    Airline::airports = airports;
}
