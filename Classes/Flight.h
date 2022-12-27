#pragma once

#include "Airport.h"
#include <string>
using namespace std;

class Flight {
private:
    string source; //airport_id
    string target; //airport_id
    string airline; //airline_id
    float distance; //in km
public:
    Flight();
    Flight(string source, string target, string airline, float distance);
    Flight(Airport source, Airport target, string airline);

    const string &getSource() const;
    void setSource(const string &source);

    const string &getTarget() const;

    void setTarget(const string &target);

    const string &getAirline() const;

    void setAirline(const string &airline);

    float getDistance() const;

    void setDistance(float distance);
};
