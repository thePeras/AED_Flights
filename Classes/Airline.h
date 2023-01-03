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
    vector<Flight*> flights;
    set<string> airports;

    public:
    Airline(){
        code = "";
        name = "";
        callSign = "";
        country = "";
        flights = {};
        airports = {};
    }

    Airline(string code, string name, string callSign, string country){
        this->code = code;
        this->name = name;
        this->callSign = callSign;
        this->country = country;
        this->flights = {};
        this->airports = {};
    }

    const string &getCode() const {
        return code;
    }

    void setCode(const string &code) {
        this->code = code;
    }

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        this->name = name;
    }

    const string &getCallSign() const {
        return callSign;
    }

    void setCallSign(const string &callSign) {
        this->callSign = callSign;
    }

    const string &getCountry() const {
        return country;
    }

    void setCountry(const string &country) {
        this->country = country;
    }

    void addFlight(Flight* flight){
        flights.push_back(flight);
    }

    vector<Flight*> getFlights(){
        return flights;
    }

    void addAirport(string airport){
        airports.insert(airport);
    }

    set<string> getAirports(){
        return airports;
    }
};

