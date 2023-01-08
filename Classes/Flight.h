#pragma once

#include <string>
#include <vector>

using namespace std;

class Flight {
private:
    string source; //airport_id
    string target; //airport_id
    float distance; //in km
    vector<string> airlines; //airline_id
public:
    Flight();

    Flight(string source, string target, float distance);

    const string &getSource() const;

    void setSource(const string &source);

    const string &getTarget() const;

    void setTarget(const string &target);

    float getDistance() const;

    void setDistance(float distance);

    void addAirline(string airline);

    const vector<string> &getAirlines() const;
};
