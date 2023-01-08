#pragma once

#include "Flight.h"
#include "Airport.h"
#include <string>
#include <vector>
#include <set>

using namespace std;

class Airline {
private:
    string code;
    string name;
    string callSign;
    string country;
    vector<Flight *> flights;
    set<string> airports;

public:
    Airline();

    Airline(string code, string name, string callSign, string country);

    void addFlight(Flight *flight);

    void addAirport(string airport);

    const string &getCode() const;

    void setCode(const string &code);

    const string &getName() const;

    void setName(const string &name);

    const string &getCallSign() const;

    void setCallSign(const string &callSign);

    const string &getCountry() const;

    void setCountry(const string &country);

    const vector<Flight *> &getFlights() const;

    void setFlights(const vector<Flight *> &flights);

    const set<string> &getAirports() const;

    void setAirports(const set<string> &airports);


};

