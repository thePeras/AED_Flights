#include "Flight.h"

using namespace std;

Flight::Flight() {
    source = "";
    target = "";
    distance = 0;
}

Flight::Flight(string source, string target, float distance) {
    this->source = source;
    this->target = target;
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

float Flight::getDistance() const {
    return distance;
}

void Flight::setDistance(float distance) {
    Flight::distance = distance;
}

void Flight::addAirline(string airline) {
    this->airlines.push_back(airline);
}

const vector<string> &Flight::getAirlines() const {
    return airlines;
}
