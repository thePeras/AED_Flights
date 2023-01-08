#pragma once

#include <string>

using namespace std;

class Validate {
public:
    static bool latitude(string latitude);

    static bool longitude(string longitude);

    static bool radius(string radius);
};

bool Validate::latitude(string input) {
    try {
        double latitude = stod(input);
        if (latitude >= -90 and latitude <= 90) return true;
        return false;
    } catch (exception e) {
        return false;
    }
}

bool Validate::longitude(string input) {
    try {
        double longitude = stod(input);
        if (longitude >= -180 and longitude <= 190) return true;
        return false;
    } catch (exception e) {
        return false;
    }
}

bool Validate::radius(string input) {
    double MAX_RADIUS = 40075; // Perímetro da circunferencia do Equador
    try {
        double radius = stod(input);
        if (radius > 0 and radius <= MAX_RADIUS) return true;
        return false;
    } catch (exception e) {
        return false;
    }
}