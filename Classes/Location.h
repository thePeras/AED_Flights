#pragma once

class Location {
    float latitude;
    float longitude;
public:
    Location();

    Location(float lat, float lon);

    float distance(Location location);

    float getLatitude() const;

    float getLongitude() const;
};
