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
};