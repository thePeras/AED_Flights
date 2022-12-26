#pragma once

#include <string>
#include <set>
#include "Airline.h"
#include "Comparators.cpp"
using namespace std;

class Managing {
public:
    Managing();

    /**
     * @brief
     * Lê todos os ficheiros: estudantes, horários, classes/UCs, requests e rejected requests.
     */
    void readFiles();

    const set<Airline, airlineComp> &getAirlines() const;

    void setAirlines(const set<Airline, airlineComp> &airlines);

private:
    //TODO: Add sets, lists, vectors (or whatever we want to use) to store the data

    void readAirlines();
    void readAirports();
    void readFlights();


    set<Airline, airlineComp> airlines;
};

