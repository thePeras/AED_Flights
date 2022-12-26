#include "Airline.h"

struct airlineComp{
    bool operator()(const Airline& a, const Airline& b) const{
        return a.getName() < b.getName();
    }
};