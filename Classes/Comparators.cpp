#include "Airline.h"

struct airlineComp{
    bool operator()(const Airline& a, const Airline& b) const{
        if(a.getName() == b.getName()){
            return a.getCode() < b.getCode();
        }
        return a.getName() < b.getName();
    }
};