#include "Location.h"

#include <cmath>

using namespace std;

Location::Location() {
    latitude = 0;
    longitude = 0;
}

Location::Location(float lat, float lon) {
    latitude = lat;
    longitude = lon;
}

float toRadians(float degrees) {
    return degrees * M_PI / 180;
}

// Returns distance between two locations in kilometers
float Location::distance(Location location) {
    float EARTH_RADIUS = 6371.0;

    float dLat = toRadians(location.latitude - this->latitude);
    float dLon = toRadians(location.longitude - this->longitude);

    float lat1 = toRadians(latitude);
    float lat2 = toRadians(location.latitude);

    // Haversine formula
    float a = pow(sin(dLat / 2), 2) +
              pow(sin(dLon / 2), 2) *
              cos(lat1) * cos(lat2);
    float c = 2 * asin(sqrt(a));

    return c * EARTH_RADIUS;
}

float Location::getLongitude() const {
    return longitude;
}

float Location::getLatitude() const {
    return latitude;
}
