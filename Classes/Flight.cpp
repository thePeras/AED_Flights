#include "Flight.h"

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

const string &Flight::getSource() const {
    return source;
}

void Flight::setSource(const string &source) {
    Flight::source = source;
}

const string &Flight::getTarget() const {
    return target;
}

void Flight::setTarget(const string &target) {
    Flight::target = target;
}

void Flight::setAirline(const string &airline) {
    Flight::airline = airline;
}

float Flight::getDistance() const {
    return distance;
}

void Flight::setDistance(float distance) {
    Flight::distance = distance;
}

const string Flight::getAirline() const {
    return airline;
}