#include "Flight.h"
#include "Airport.h"

using namespace std;

Flight::Flight() {
    source = "";
    target = "";
    airline = "";
    distance = 0;
}

Flight::Flight(string source, string target, string airline, float distance) {
    this->source = source;
    this->target = target;
    this->airline = airline;
    this->distance = distance;
}

Flight::Flight(Airport source, Airport target, string airline) {
    this->source = source.getCode();
    this->target = target.getCode();
    this->airline = airline;
    this->distance = source.getLocation().distance(target.getLocation());
}

const string &Flight::getSource() const {
    return source;
}

void Flight::setSource(const string &source) {
    Flight::source = source;
}
